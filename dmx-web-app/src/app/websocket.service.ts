import { Injectable } from '@angular/core';
import { WebSocketSubject, webSocket } from 'rxjs/webSocket';

export interface WebSocketSubscriber {
  onMsg(msg: any):void;
}

@Injectable({
  providedIn: 'root'
})
export class WebSocketService {

  webSocket: WebSocketSubject<any>;
  listOfSubscribers: WebSocketSubscriber[] = [];

  constructor() {
    fixWebSocketError();
    this.webSocket = webSocket("ws://artnet.local/ws");

    console.log("subscribe");
    this.webSocket.subscribe({
      next: msg => this.onMsg(msg),
      error: err => console.log(err), // Called if at any point WebSocket API signals some kind of error.
      complete: () => console.log('complete') // Called when connection is closed (for whatever reason).
    });
  }

  onMsg(msg: any): void {
    for (const subscriber of this.listOfSubscribers) {
      subscriber.onMsg(msg);
    }
  }

  subscribe(subscriberObject: WebSocketSubscriber) {
    this.listOfSubscribers.push(subscriberObject);
  }

  send() {
    console.log("reset");
    this.webSocket.next({status:{state: "reset"}});
  }
}

function fixWebSocketError() {
  // https://stackoverflow.com/questions/51532159/referenceerror-websocket-is-not-defined-when-using-rxjs-websocketsubject-and
  if (typeof global !== 'undefined') {
    (global as any).WebSocket = require('ws');
  }
}

