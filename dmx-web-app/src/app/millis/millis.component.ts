import { Component, Input, Output, EventEmitter } from '@angular/core';
import { CommonModule } from '@angular/common';
import { WebSocketService, WebSocketSubscriber } from '../websocket.service';

@Component({
  selector: 'app-millis',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './millis.component.html',
})

export class MillisComponent implements WebSocketSubscriber{

  @Input() model: any;
  @Output() modelChange = new EventEmitter<any>();

  constructor(private webSocketService: WebSocketService) {}

  onMsg(msg: any) {
    console.log('message received: ' + JSON.stringify(msg.status.uptime_millis)); // Called whenever there is a message from the server.
    this.model.status.uptime_millis = msg.status.uptime_millis;
    this.modelChange.emit(this.model);
  }

  ngOnInit() {
    this.webSocketService.subscribe(this);
  }
}
