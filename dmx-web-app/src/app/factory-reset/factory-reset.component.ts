import { Component } from '@angular/core';
import { WebSocketService } from '../websocket.service';

@Component({
  selector: 'app-factory-reset',
  standalone: true,
  imports: [],
  templateUrl: './factory-reset.component.html'
})
export class FactoryResetComponent {

  constructor(private webSocketService: WebSocketService) {}

  onClick() {
    this.webSocketService.send();
  }
}
