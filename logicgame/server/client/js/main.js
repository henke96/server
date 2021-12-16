class Main {
    constructor() {
        this.currentView = null;
        this.socket = null;
        this.connected = false;
        this.buffer = new ArrayBuffer(1024);
        this.bufferByteView = new Uint8Array(this.buffer);
        this.bufferDataView = new DataView(this.buffer);

        this.gameView = new GameView();
        this.tryConnect = () => {
            this.onSocketClose = (event) => {
                this.clearSocketListeners();
                if (this.connected) {
                    this.connected = false;
                    this.setView(this.connectView);
                } else {
                    this.connectView.onFail();
                }
            };
            this.onSocketMessage = (event) => {
                try {
                    let receiveData = new DataView(event.data);
                    if (!this.connected) {
                        let version = receiveData.getUint32(0, true);
                        if (version === protocolVersion) {
                            this.connected = true;
                        } else {
                            // Perhaps the game updated, reload page.
                            console.log("Reloading page...");
                            setTimeout(() => {
                                location.reload();
                            }, 1000);
                            this.clearSocketListeners();
                        }
                        return;
                    }

                    let op = receiveData.getUint8(0);
                    switch (op) {
                        case ProtocolServerView.GAME:
                            this.gameView.update(receiveData, 1);
                            this.setView(this.gameView);
                            break;
                        default: throw "Invalid view!";
                    }
                } catch (error) {
                    console.error("Error handling server message:", error);
                    // TODO what do on errors?
                }
            };
            let protoPrefix = (location.protocol === "https:") ? "wss://" : "ws://";
            this.socket = new WebSocket(protoPrefix + location.host);
            this.socket.binaryType = "arraybuffer";
            this.socket.addEventListener("close", this.onSocketClose);
            this.socket.addEventListener("message", this.onSocketMessage);
        }
        this.connectView = new ConnectView(this.tryConnect);
    }
    setView(newView) {
        if (newView === this.currentView) return;

        if (this.currentView) this.currentView.close();
        this.currentView = newView;
        if (this.currentView) this.currentView.open();
    }

    clearSocketListeners() {
        this.socket.removeEventListener("message", this.onSocketMessage);
        this.socket.removeEventListener("close", this.onSocketClose);
    }
}

window.onload = function() {
    let main = new Main();
    main.setView(main.connectView);
};