import { Component } from '@angular/core';
import { BehaviorSubject } from 'rxjs';
import { CommonModule } from '@angular/common';
import { WebSocketService, WebSocketSubscriber } from '../websocket.service';

@Component({
  selector: 'app-millis',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './millis.component.html',
})

export class MillisComponent implements WebSocketSubscriber{
  millis$ = new BehaviorSubject(678);
  constructor(private webSocketService: WebSocketService) {}

  onMsg(msg: any) {
    console.log('message received: ' + JSON.stringify(msg.status.uptime_millis)); // Called whenever there is a message from the server.
    this.millis$.next(msg.status.uptime_millis);
  }

  ngOnInit() {
    this.webSocketService.subscribe(this);
  }
}
