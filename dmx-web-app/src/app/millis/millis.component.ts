import { Component } from '@angular/core';
import { webSocket } from 'rxjs/webSocket';
import { BehaviorSubject } from 'rxjs';
import { CommonModule } from '@angular/common';

@Component({
  selector: 'app-millis',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './millis.component.html',
})

export class MillisComponent {
  millis$ = new BehaviorSubject(678);

  onMsg(msg: any) {
    console.log('message received: ' + msg.millis); // Called whenever there is a message from the server.
    this.millis$.next(msg.millis);
  }

  ngOnInit() {
    // TODO: read node name from config
    // https://github.com/me-no-dev/ESPAsyncWebServer?tab=readme-ov-file#template-processing
    const subject = webSocket('ws:///ws');

    subject.subscribe({
      next: msg => this.onMsg(msg),
      error: err => console.log(err), // Called if at any point WebSocket API signals some kind of error.
      complete: () => console.log('complete') // Called when connection is closed (for whatever reason).
    });

  }
}
