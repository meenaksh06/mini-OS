const express = require('express');
const http = require('http');
const WebSocket = require('ws');
const pty = require('node-pty');
const path = require('path');

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

app.use(express.static(path.join(__dirname, 'public')));

app.get('/health', (_req, res) => res.json({ status: 'ok', ts: Date.now() }));

wss.on('connection', (ws) => {
    const binaryPath = path.join(__dirname, '..', 'mini-os');

    let shell;
    try {
        // Spawn via /bin/sh so posix_spawnp always succeeds, then exec into the binary.
        // This avoids node-pty's direct posix_spawnp call failing on macOS for custom binaries.
        shell = pty.spawn('/bin/sh', ['-c', `exec '${binaryPath}'`], {
            name: 'xterm-256color',
            cols: 100,
            rows: 30,
            cwd: path.join(__dirname, '..'),
            env: { ...process.env, TERM: 'xterm-256color', COLORTERM: 'truecolor' },
        });
    } catch (err) {
        ws.send(JSON.stringify({
            type: 'error',
            data: `\r\n\x1b[1;31m[Error] Could not spawn mini-os: ${err.message}\x1b[0m\r\n`
                + `\x1b[33mMake sure you built the binary first:\x1b[0m\r\n`
                + `\x1b[36m  cd .. && gcc -I. main.c math/math.c string/string.c memory/memory.c screen/screen.c keyboard/keyboard.c security/security.c vfs/vfs.c process/process.c -o mini-os\x1b[0m\r\n`,
        }));
        return;
    }

    shell.on('data', (data) => {
        if (ws.readyState === WebSocket.OPEN)
            ws.send(JSON.stringify({ type: 'output', data }));
    });

    ws.on('message', (raw) => {
        try {
            const msg = JSON.parse(raw);
            if (msg.type === 'input') shell.write(msg.data);
            else if (msg.type === 'resize') shell.resize(msg.cols, msg.rows);
        } catch (_) {}
    });

    shell.on('exit', (code) => {
        if (ws.readyState === WebSocket.OPEN)
            ws.send(JSON.stringify({ type: 'exit', code }));
    });

    ws.on('close', () => { try { shell.kill(); } catch (_) {} });
    ws.on('error', () => { try { shell.kill(); } catch (_) {} });
});

const PORT = process.env.PORT || 3000;
server.listen(PORT, () => {
    console.log(`\n  ╔══════════════════════════════════════╗`);
    console.log(`  ║   Mini-OS UI → http://localhost:${PORT}   ║`);
    console.log(`  ╚══════════════════════════════════════╝\n`);
});
