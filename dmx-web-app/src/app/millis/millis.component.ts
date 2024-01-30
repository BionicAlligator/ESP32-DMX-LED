import { Component } from '@angular/core';
import { webSocket } from 'rxjs/webSocket';

@Component({
  selector: 'app-millis',
  standalone: true,
  imports: [],
  templateUrl: './millis.component.html',
  styleUrl: './millis.component.css'
})

export class MillisComponent {
  millis$ = new BehaviorSubject(456);

  onMsg(msg: any) {
    console.log('message received: ' + msg.millis) // Called whenever there is a message from the server.
    this.millis$.next(msg.millis);
  }

  ngOnInit() {
    const subject = webSocket('ws://artnet.local:80/ws');

    subject.subscribe({
      next: this.onMsg,
      error: err => console.log(err), // Called if at any point WebSocket API signals some kind of error.
      complete: () => console.log('complete') // Called when connection is closed (for whatever reason).
    });

  }
}
