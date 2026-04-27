#!/usr/bin/env python3
"""Mini-OS PTY server — uses Python's built-in pty module (no native addons)."""
import asyncio
import fcntl
import json
import os
import pty
import struct
import subprocess
import termios
import threading
from http.server import HTTPServer, SimpleHTTPRequestHandler

import websockets

ROOT   = os.path.dirname(os.path.abspath(__file__))
BINARY = os.path.abspath(os.path.join(ROOT, "..", "mini-os"))
STATIC = os.path.join(ROOT, "public")
HTTP_PORT = 3000
WS_PORT   = 3001


class _Static(SimpleHTTPRequestHandler):
    def __init__(self, *a, **kw):
        super().__init__(*a, directory=STATIC, **kw)
    def log_message(self, *_):
        pass


def _run_http():
    HTTPServer(("", HTTP_PORT), _Static).serve_forever()


async def handle(ws):
    # Open a real PTY pair — slave side becomes the terminal for mini-os
    master_fd, slave_fd = pty.openpty()
    fcntl.ioctl(slave_fd, termios.TIOCSWINSZ, struct.pack("HHHH", 30, 100, 0, 0))

    try:
        proc = subprocess.Popen(
            [BINARY],
            stdin=slave_fd, stdout=slave_fd, stderr=slave_fd,
            preexec_fn=os.setsid,
            close_fds=True,
        )
    except Exception as e:
        await ws.send(json.dumps({
            "type": "error",
            "data": f"\r\n\x1b[1;31m[Error] Cannot start mini-os: {e}\x1b[0m\r\n"
                    f"\x1b[33mRebuild:\x1b[0m\r\n"
                    f"\x1b[36m  cd .. && gcc -I. main.c math/math.c string/string.c "
                    f"memory/memory.c screen/screen.c keyboard/keyboard.c "
                    f"security/security.c vfs/vfs.c process/process.c -o mini-os\x1b[0m\r\n",
        }))
        os.close(slave_fd)
        os.close(master_fd)
        return

    os.close(slave_fd)   # parent no longer needs the slave end

    loop = asyncio.get_running_loop()
    stop = asyncio.Event()

    # Forward PTY output → WebSocket
    def _on_readable():
        try:
            data = os.read(master_fd, 4096)
            loop.create_task(
                ws.send(json.dumps({
                    "type": "output",
                    "data": data.decode("utf-8", errors="replace"),
                }))
            )
        except OSError:
            loop.remove_reader(master_fd)
            stop.set()

    loop.add_reader(master_fd, _on_readable)

    # Forward WebSocket input → PTY
    async def _recv():
        try:
            async for raw in ws:
                m = json.loads(raw)
                if m["type"] == "input":
                    try:
                        os.write(master_fd, m["data"].encode("utf-8"))
                    except OSError:
                        break
                elif m["type"] == "resize":
                    try:
                        fcntl.ioctl(
                            master_fd, termios.TIOCSWINSZ,
                            struct.pack("HHHH", m["rows"], m["cols"], 0, 0),
                        )
                    except OSError:
                        pass
        except Exception:
            pass
        stop.set()

    recv_task = loop.create_task(_recv())
    await stop.wait()
    recv_task.cancel()

    loop.remove_reader(master_fd)
    try:
        proc.terminate()
    except Exception:
        pass
    try:
        os.close(master_fd)
    except Exception:
        pass


async def _main():
    threading.Thread(target=_run_http, daemon=True).start()

    print(f"\n  ╔════════════════════════════════════════╗")
    print(f"  ║  Mini-OS UI  →  http://localhost:{HTTP_PORT}   ║")
    print(f"  ╚════════════════════════════════════════╝\n")

    async with websockets.serve(handle, "localhost", WS_PORT):
        await asyncio.Future()


if __name__ == "__main__":
    asyncio.run(_main())
