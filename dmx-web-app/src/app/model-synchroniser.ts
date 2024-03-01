import { Injectable } from '@angular/core';
import { WebSocketSubject, webSocket } from 'rxjs/webSocket';
import { DmxNodeModel } from './dmx-node-model';
import { BehaviorSubject } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class ModelSynchroniser {
  // sends and receives web socket messages when needed, based on model changes

  modelChangesFromServer$ = new BehaviorSubject<DmxNodeModel>({ status: { uptime_millis: 1234 } });  //Potentially use ReplaySubject instead

  // millis_http$ = this.httpClient.get('/millis').pipe(shareReplay(1)) // How to avoid multiple HTTP requests if you are using a REST pattern instead of websockets
  webSocket$: WebSocketSubject<any>;
  constructor() {
    fixWebSocketError();
    this.webSocket$ = webSocket("ws://artnet.local/ws");

    console.log("subscribe");
    this.webSocket$.subscribe({
      next: msg => this.onMsg(msg),
      error: err => console.log(err), // Called if at any point WebSocket API signals some kind of error.
      complete: () => console.log('Model Synchroniser Subscription to webSocket$ complete') // Called when connection is closed (for whatever reason).
    });
  }

  onMsg(msg: any) {
    console.log('Message Received: ' + JSON.stringify(msg)); // Called whenever there is a message from the server.
    // is msg the whole model?  probably not, but we assume so for now
    this.modelChangesFromServer$.next(msg)
  }

  send(modelChange: any) {
    console.log("Model Changed: " + JSON.stringify(modelChange));
    this.webSocket$.next(modelChange);
  }
}

function fixWebSocketError() {
  // https://stackoverflow.com/questions/51532159/referenceerror-websocket-is-not-defined-when-using-rxjs-websocketsubject-and
  if (typeof global !== 'undefined') {
    (global as any).WebSocket = require('ws');
  }
}

