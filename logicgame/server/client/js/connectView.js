class ConnectView {
    constructor(tryConnect) {
        this.tryConnect = tryConnect

        this.div = document.getElementById("connectView");
        this.connectingText = document.getElementById("connectViewConnecting");
        this.reconnectDiv = document.getElementById("connectViewReconnect");
        this.retryButton = document.getElementById("connectViewRetry");
    }
    onFail() {
        this.onRetry = (event) => {
            this.retryButton.removeEventListener("click", this.onRetry);
            this.connect();
        }
        this.retryButton.addEventListener("click", this.onRetry);

        this.connectingText.classList.add("hidden");
        this.reconnectDiv.classList.remove("hidden");
    }
    connect() {
        this.reconnectDiv.classList.add("hidden");
        this.connectingText.classList.remove("hidden");
        this.tryConnect();
    }
    open() {
        this.div.classList.remove("hidden");
        this.connect();
    }
    close() {
        this.div.classList.add("hidden");
        this.connectingText.classList.add("hidden");
        this.reconnectDiv.classList.add("hidden");
    }
}