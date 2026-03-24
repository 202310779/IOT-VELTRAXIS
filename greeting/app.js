const WS_URL = "ws://localhost:8765";
const GREET_DURATION = 5000; // How long the greeting screen stays visible (ms)

const idleScreen  = document.getElementById("idle-screen");
const greetScreen = document.getElementById("greet-screen");
const audio       = document.getElementById("greet-audio");

let greetTimer = null;

function showGreeting() {
  // Clear any existing timer
  if (greetTimer) clearTimeout(greetTimer);

  // Switch screens
  idleScreen.classList.remove("active");
  greetScreen.classList.add("active");

  // Play audio
  audio.currentTime = 0;
  audio.play().catch(() => {
    // Autoplay blocked — user needs to interact with the page first
    console.warn("Audio autoplay blocked. Click the page once to enable.");
  });

  // Return to idle after GREET_DURATION
  greetTimer = setTimeout(() => {
    greetScreen.classList.remove("active");
    idleScreen.classList.add("active");
  }, GREET_DURATION);
}

function connect() {
  const ws = new WebSocket(WS_URL);

  ws.onopen = () => console.log("[WS] Connected to bridge.");

  ws.onmessage = (event) => {
    if (event.data === "GREET") showGreeting();
  };

  ws.onclose = () => {
    console.warn("[WS] Disconnected. Retrying in 3s...");
    setTimeout(connect, 3000); // Auto-reconnect
  };

  ws.onerror = (err) => console.error("[WS] Error:", err);
}

// Unlock audio on first user interaction (browser autoplay policy)
document.addEventListener("click", () => audio.load(), { once: true });

connect();
