/* ─── Terminal Setup ─── */
const term = new Terminal({
  theme: {
    background:      '#07070e',
    foreground:      '#e2e8f0',
    cursor:          '#6366f1',
    cursorAccent:    '#07070e',
    selectionBackground: 'rgba(99,102,241,.3)',

    black:        '#1e1e30',
    red:          '#f43f5e',
    green:        '#10b981',
    yellow:       '#f59e0b',
    blue:         '#6366f1',
    magenta:      '#a855f7',
    cyan:         '#22d3ee',
    white:        '#e2e8f0',
    brightBlack:  '#475569',
    brightRed:    '#fb7185',
    brightGreen:  '#34d399',
    brightYellow: '#fcd34d',
    brightBlue:   '#818cf8',
    brightMagenta:'#c084fc',
    brightCyan:   '#67e8f9',
    brightWhite:  '#f8fafc',
  },
  fontFamily: '"JetBrains Mono", "Fira Code", monospace',
  fontSize: 13,
  lineHeight: 1.55,
  letterSpacing: 0.4,
  cursorBlink: true,
  cursorStyle: 'bar',
  cursorWidth: 2,
  scrollback: 2000,
  allowProposedApi: true,
  convertEol: true,
});

const fitAddon = new FitAddon.FitAddon();
term.loadAddon(fitAddon);
term.open(document.getElementById('terminal'));

function fit() {
  try { fitAddon.fit(); } catch (_) {}
}
fit();

/* ─── WebSocket — Python PTY server on port 3001 ─── */
const ws = new WebSocket('ws://localhost:3001');

const connDot  = document.getElementById('conn-dot');
const connText = document.getElementById('conn-text');

function setConn(state, label) {
  connDot.className = 'conn-dot' + (state ? ` ${state}` : '');
  connText.textContent = label;
}

ws.onopen  = () => { setConn('ok',  'Connected'); term.focus(); };
ws.onclose = () => setConn('err', 'Disconnected');
ws.onerror = () => setConn('err', 'Error');

ws.onmessage = ({ data: raw }) => {
  const msg = JSON.parse(raw);
  if      (msg.type === 'output') term.write(msg.data);
  else if (msg.type === 'error')  term.write(msg.data);
  else if (msg.type === 'exit') {
    term.write('\r\n\x1b[1;33m[Process exited — refresh to restart]\x1b[0m\r\n');
    setConn('err', 'Exited');
  }
};

/* ─── Input ─── */
term.onData((data) => {
  if (ws.readyState === WebSocket.OPEN)
    ws.send(JSON.stringify({ type: 'input', data }));
});

/* ─── Resize ─── */
const ro = new ResizeObserver(() => {
  fit();
  if (ws.readyState === WebSocket.OPEN)
    ws.send(JSON.stringify({ type: 'resize', cols: term.cols, rows: term.rows }));
});
ro.observe(document.getElementById('terminal'));
window.addEventListener('resize', fit);

/* ─── Quick-command buttons ─── */
document.querySelectorAll('.cmd-btn').forEach((btn) => {
  btn.addEventListener('click', () => {
    if (ws.readyState !== WebSocket.OPEN) return;
    ws.send(JSON.stringify({ type: 'input', data: btn.dataset.cmd + '\r' }));
    term.focus();
  });
});

/* ─── AI demo buttons (send the typo so the shell shows correction) ─── */
document.querySelectorAll('.ai-demo-btn').forEach((btn) => {
  btn.addEventListener('click', () => {
    if (ws.readyState !== WebSocket.OPEN) return;
    ws.send(JSON.stringify({ type: 'input', data: btn.dataset.cmd + '\r' }));
    term.focus();
  });
});

/* ─── Uptime counter ─── */
const t0 = Date.now();
const uptimeEl = document.getElementById('uptime');
setInterval(() => {
  const s = Math.floor((Date.now() - t0) / 1000);
  const h = String(Math.floor(s / 3600)).padStart(2, '0');
  const m = String(Math.floor((s % 3600) / 60)).padStart(2, '0');
  const sc = String(s % 60).padStart(2, '0');
  uptimeEl.textContent = `${h}:${m}:${sc}`;
}, 1000);

/* ─── Focus on click anywhere in terminal area ─── */
document.getElementById('terminal').addEventListener('click', () => term.focus());
