// Scan and display available WiFi networks with signal strengths on a HTML Webpage
// Type 192.168.4.1 on the browser (as defined in the following code) to open the HTML webpage

#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

String scannedNetworks = "";
String connectStatus   = "";
String connectSSID     = "";

// ─── Scan Networks ────────────────────────────────────────────────
void scanNetworks() {
  scannedNetworks = "";
  int n = WiFi.scanNetworks();
  if (n == 0) {
    scannedNetworks = "<p class='none'>No networks found. Try rescanning.</p>";
    return;
  }
  for (int i = 0; i < n; ++i) {
    String enc;
    switch (WiFi.encryptionType(i)) {
      case WIFI_AUTH_OPEN:            enc = "🔓 Open";            break;
      case WIFI_AUTH_WEP:             enc = "🔒 WEP";             break;
      case WIFI_AUTH_WPA_PSK:         enc = "🔒 WPA";             break;
      case WIFI_AUTH_WPA2_PSK:        enc = "🔒 WPA2";            break;
      case WIFI_AUTH_WPA_WPA2_PSK:    enc = "🔒 WPA/WPA2";        break;
      case WIFI_AUTH_WPA2_ENTERPRISE: enc = "🔒 WPA2-Ent";        break;
      case WIFI_AUTH_WPA3_PSK:        enc = "🔒 WPA3";            break;
      default:                        enc = "❓ Unknown";
    }
    int    rssi    = WiFi.RSSI(i);
    int    channel = WiFi.channel(i);
    String ssid    = WiFi.SSID(i);

    String sigLabel, sigColor;
    if      (rssi > -50) { sigLabel = "Excellent"; sigColor = "#1d6fe8"; }
    else if (rssi > -65) { sigLabel = "Good";      sigColor = "#3b82f6"; }
    else if (rssi > -75) { sigLabel = "Fair";      sigColor = "#93c5fd"; }
    else                 { sigLabel = "Weak";      sigColor = "#bfdbfe"; }

    String bar = rssi > -50 ? "▂▄▆█" : rssi > -65 ? "▂▄▆_" : rssi > -75 ? "▂▄__" : "▂___";

    scannedNetworks +=
      "<div class='net' onclick=\"openModal('" + ssid + "')\">"
        "<div class='net-icon'>📶</div>"
        "<div class='net-mid'>"
          "<span class='ssid'>" + ssid + "</span>"
          "<span class='meta'>Ch " + String(channel) + " &nbsp;·&nbsp; " + enc + "</span>"
        "</div>"
        "<div class='net-right'>"
          "<span class='bar' style='color:" + sigColor + "'>" + bar + "</span>"
          "<span class='sig' style='color:" + sigColor + "'>" + sigLabel + "</span>"
          "<span class='rssi'>" + String(rssi) + " dBm</span>"
        "</div>"
      "</div>";
  }
}

// ══════════════════════════════════════════════════════════════════
// PAGE 1 — HOME
// ══════════════════════════════════════════════════════════════════
String buildHomePage() {
  String connBox = "";
  if (WiFi.status() == WL_CONNECTED) {
    connBox =
      "<div class='conn-box'>"
        "<div class='conn-icon'>✅</div>"
        "<div><div class='conn-ssid'>" + WiFi.SSID() + "</div>"
        "<div class='conn-ip'>IP: " + WiFi.localIP().toString() + "</div></div>"
      "</div>";
  }

  return R"html(<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>ESP32-C3 WiFi Manager</title>
<style>
  * { box-sizing: border-box; margin: 0; padding: 0; }
  body {
    font-family: 'Segoe UI', sans-serif;
    background: #f0f4ff;
    color: #1e2a45;
    min-height: 100vh;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    padding: 30px 20px;
  }
  .logo { font-size: 4rem; margin-bottom: 10px; }
  h1 {
    font-size: 1.7rem;
    color: #1d4ed8;
    font-weight: 700;
    text-align: center;
    letter-spacing: -0.3px;
  }
  .tagline {
    color: #6b7eb8;
    font-size: 0.88rem;
    text-align: center;
    margin-top: 6px;
    margin-bottom: 32px;
  }
  .conn-box {
    display: flex; align-items: center; gap: 14px;
    background: #eff6ff;
    border: 1.5px solid #3b82f6;
    border-radius: 14px; padding: 14px 20px;
    margin-bottom: 28px; width: 100%; max-width: 360px;
    box-shadow: 0 2px 12px rgba(59,130,246,0.12);
  }
  .conn-icon { font-size: 1.8rem; }
  .conn-ssid { font-weight: 700; color: #1d4ed8; font-size: 1rem; }
  .conn-ip   { font-size: 0.8rem; color: #6b7eb8; margin-top: 2px; }
  .btn-scan {
    display: block; width: 100%; max-width: 360px;
    padding: 16px; border-radius: 14px; border: none;
    background: linear-gradient(135deg, #1d4ed8, #3b82f6);
    color: #fff; font-size: 1.05rem; font-weight: 700;
    cursor: pointer; letter-spacing: 0.4px;
    box-shadow: 0 4px 20px rgba(59,130,246,0.35);
    transition: transform 0.15s, opacity 0.15s;
  }
  .btn-scan:hover  { opacity: 0.9; transform: translateY(-1px); }
  .btn-scan:active { transform: translateY(1px); }
  .hint { color: #9aaac8; font-size: 0.78rem; margin-top: 14px; text-align: center; }
</style>
</head>
<body>

<div class="logo">📶</div>
<h1>ESP32-C3 WiFi Manager</h1>
<p class="tagline">Manage your WiFi connection from the browser</p>

)html" + connBox + R"html(

<button class="btn-scan" onclick="location.href='/networks'">
  🔍 &nbsp; Scan Networks
</button>

<p class="hint">Tap to discover available WiFi networks nearby</p>

</body>
</html>)html";
}

// ══════════════════════════════════════════════════════════════════
// PAGE 2 — NETWORKS
// ══════════════════════════════════════════════════════════════════
String buildNetworksPage() {
  String statusMsg = "";
  if (connectStatus != "") {
    bool ok  = connectStatus.startsWith("✅");
    String c = ok ? "#1d6fe8" : "#dc2626";
    String bg = ok ? "#eff6ff" : "#fff1f2";
    statusMsg = "<div class='status-bar' style='border-color:" + c + ";color:" + c + ";background:" + bg + "'>" + connectStatus + "</div>";
  }

  return R"html(<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Networks — ESP32-C3</title>
<style>
  * { box-sizing: border-box; margin: 0; padding: 0; }
  body {
    font-family: 'Segoe UI', sans-serif;
    background: #f0f4ff;
    color: #1e2a45;
    min-height: 100vh;
    padding: 20px;
  }
  .topbar {
    display: flex; align-items: center; gap: 12px;
    margin-bottom: 20px;
  }
  .back-btn {
    background: #ffffff;
    border: 1.5px solid #bfdbfe;
    border-radius: 10px;
    color: #1d4ed8;
    padding: 8px 14px;
    font-size: 0.9rem;
    font-weight: 600;
    cursor: pointer;
    text-decoration: none;
    box-shadow: 0 1px 4px rgba(59,130,246,0.1);
    transition: background 0.2s;
  }
  .back-btn:hover { background: #eff6ff; }
  .topbar h1 { font-size: 1.2rem; color: #1d4ed8; font-weight: 700; }
  .status-bar {
    border: 1.5px solid;
    border-radius: 10px;
    padding: 10px 14px;
    margin-bottom: 14px;
    font-size: 0.88rem;
    font-weight: 600;
    text-align: center;
  }
  .net {
    display: flex; align-items: center; gap: 12px;
    background: #ffffff;
    border: 1.5px solid #dbeafe;
    border-radius: 12px; padding: 13px 14px;
    margin-bottom: 10px; cursor: pointer;
    box-shadow: 0 1px 6px rgba(59,130,246,0.07);
    transition: background 0.2s, border-color 0.2s, box-shadow 0.2s;
  }
  .net:hover {
    background: #eff6ff;
    border-color: #3b82f6;
    box-shadow: 0 3px 14px rgba(59,130,246,0.15);
  }
  .net-icon { font-size: 1.4rem; }
  .net-mid  { flex: 1; }
  .ssid     { font-weight: 600; font-size: 0.95rem; color: #1e2a45; }
  .meta     { font-size: 0.72rem; color: #7b93c0; margin-top: 3px; }
  .net-right { display: flex; flex-direction: column; align-items: flex-end; gap: 2px; }
  .bar      { font-size: 0.95rem; letter-spacing: -1px; }
  .sig      { font-size: 0.72rem; font-weight: 600; }
  .rssi     { font-size: 0.68rem; color: #9aaac8; }
  .none     { color: #9aaac8; text-align: center; padding: 20px; }
  .btn-rescan {
    display: block; width: 100%; padding: 13px; border-radius: 12px;
    border: 1.5px solid #bfdbfe;
    background: #ffffff;
    color: #1d4ed8;
    font-size: 0.95rem; font-weight: 700;
    cursor: pointer; margin-bottom: 18px;
    box-shadow: 0 1px 6px rgba(59,130,246,0.1);
    transition: background 0.2s;
  }
  .btn-rescan:hover { background: #eff6ff; }
  .overlay {
    display: none; position: fixed; inset: 0;
    background: rgba(30,42,69,0.55);
    backdrop-filter: blur(3px);
    z-index: 100;
    align-items: center; justify-content: center;
  }
  .overlay.show { display: flex; }
  .modal {
    background: #ffffff;
    border: 1.5px solid #bfdbfe;
    border-radius: 18px; padding: 28px 24px;
    width: 90%; max-width: 360px;
    box-shadow: 0 8px 40px rgba(59,130,246,0.2);
  }
  .modal h2 { font-size: 1rem; color: #1e2a45; margin-bottom: 18px; text-align: center; font-weight: 600; }
  .modal-ssid { font-weight: 700; color: #1d4ed8; }
  label { font-size: 0.8rem; color: #6b7eb8; display: block; margin-bottom: 6px; font-weight: 500; }
  input[type=password] {
    width: 100%; padding: 10px 14px; border-radius: 8px;
    border: 1.5px solid #bfdbfe;
    background: #f8faff;
    color: #1e2a45;
    font-size: 0.9rem; margin-bottom: 18px; outline: none;
    transition: border-color 0.2s;
  }
  input:focus { border-color: #3b82f6; background: #fff; }
  .modal-btns { display: flex; gap: 10px; }
  .btn-ok {
    flex: 1; padding: 11px; border-radius: 10px; border: none;
    background: linear-gradient(135deg, #1d4ed8, #3b82f6);
    color: #fff; font-weight: 700; font-size: 0.95rem; cursor: pointer;
    box-shadow: 0 3px 12px rgba(59,130,246,0.3);
    transition: opacity 0.2s;
  }
  .btn-cancel {
    flex: 1; padding: 11px; border-radius: 10px;
    border: 1.5px solid #bfdbfe;
    background: #f8faff;
    color: #1d4ed8; font-weight: 600; font-size: 0.95rem; cursor: pointer;
    transition: background 0.2s;
  }
  .btn-ok:hover     { opacity: 0.88; }
  .btn-cancel:hover { background: #eff6ff; }
</style>
</head>
<body>

<div class="topbar">
  <a class="back-btn" href="/">← Home</a>
  <h1>📡 Available Networks</h1>
</div>

)html" + statusMsg + R"html(

<button class="btn-rescan" onclick="location.href='/scan'">🔄 Rescan Networks</button>

<div id="netlist">
)html" + scannedNetworks + R"html(
</div>

<!-- Connect Modal -->
<div class="overlay" id="overlay">
  <div class="modal">
    <h2>Connect to <span class="modal-ssid" id="modal-ssid"></span></h2>
    <label>Password</label>
    <input type="password" id="pass" placeholder="Enter WiFi password">
    <div class="modal-btns">
      <button class="btn-cancel" onclick="closeModal()">Cancel</button>
      <button class="btn-ok"     onclick="doConnect()">Connect</button>
    </div>
  </div>
</div>

<script>
let selectedSSID = "";

function openModal(ssid) {
  selectedSSID = ssid;
  document.getElementById('modal-ssid').textContent = ssid;
  document.getElementById('pass').value = "";
  document.getElementById('overlay').classList.add('show');
  setTimeout(() => document.getElementById('pass').focus(), 100);
}

function closeModal() {
  document.getElementById('overlay').classList.remove('show');
}

function doConnect() {
  const pass = document.getElementById('pass').value;
  closeModal();
  fetch('/connect?ssid=' + encodeURIComponent(selectedSSID) + '&pass=' + encodeURIComponent(pass))
    .then(r => r.text())
    .then(t => { alert(t); location.reload(); });
}

document.getElementById('overlay').addEventListener('click', function(e) {
  if (e.target === this) closeModal();
});
</script>
</body>
</html>)html";
}

// ─── Route Handlers ───────────────────────────────────────────────
void handleHome() {
  server.send(200, "text/html", buildHomePage());
}

void handleNetworks() {
  server.send(200, "text/html", buildNetworksPage());
}

void handleScan() {
  connectStatus = "";
  scanNetworks();
  server.sendHeader("Location", "/networks");
  server.send(302, "text/plain", "");
}

void handleConnect() {
  String ssid = server.arg("ssid");
  String pass = server.arg("pass");

  WiFi.begin(ssid.c_str(), pass.c_str());

  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 20) {
    delay(500);
    timeout++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    connectStatus = "✅ Connected! IP: " + WiFi.localIP().toString();
    server.send(200, "text/plain", "✅ Connected! IP: " + WiFi.localIP().toString());
  } else {
    connectStatus = "❌ Failed to connect to " + ssid;
    server.send(200, "text/plain", "❌ Failed to connect. Check password.");
    WiFi.disconnect();
  }
}

// ─── Setup ────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
    // ESP32-C3 native USB CDC Wait
  unsigned long t0 = millis();
  while (!Serial && millis() - t0 < 3000) {
    delay(10);
  }

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("ESP32C3-WiFiManager", "12345678");

  Serial.println("\nAccess Point Started");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  // Do initial scan
  scanNetworks();

  // Routes
  server.on("/",         handleHome);
  server.on("/networks", handleNetworks);
  server.on("/scan",     handleScan);
  server.on("/connect",  handleConnect);

  server.begin();
  Serial.println("Web server ready -> http://192.168.4.1");
}

// ─── Loop ─────────────────────────────────────────────────────────
void loop() {
  server.handleClient();
}