

/*
- Define formal schema in an javascript interface for the model
- Switch from WebSocketService to ModelSynchroniser
- Switch back to "| async"
*/


export interface DmxNodeModel {
        "status": {
          "uptime_millis":number
        }
}



@Injectable({
    providedIn: 'root'
})
export class ModelSynchroniser implements WebSocketSubscriber {
  // sends and receives web socket messages when needed, based on model changes

    model$ = new BehaviorSubject<DmxNodeModel>();
    // millis_http$ = this.httpClient.get('/millis').pipe(shareReplay(1)) // How to avoid multiple HTTP requests if you are using a REST pattern instead of websockets

    constructor(private webSocketService: WebSocketService) { }

    onMsg(msg: any) {
        console.log('asdfasdfmessage received: ' + JSON.stringify(msg)); // Called whenever there is a message from the server.
        // is msg the whole model?  problably not, but we assume so for now
        this.model$.next(msg)
    }

    ngOnInit() {
        this.webSocketService.subscribe(this);
    }

    reset() {
        this.webSocketService.send()
    }
}




import { Component, Input, Output, EventEmitter } from '@angular/core';
import { CommonModule } from '@angular/common';
import { WebSocketService, WebSocketSubscriber } from '../websocket.service';

@Component({
  selector: 'app-millis',
  standalone: true,
  imports: [CommonModule],
  template: `
    <p>millis is {{ model.status.uptime_millis }}</p>
    <button (click)="onClick();">Reset</button>
  `
})

export class MillisComponent {
    @Input() model: MillisModel;
    @Output() modelChange = new EventEmitter<MillisModel>();

    onClick() {
      modelChange.emit();
    }
}




import { Component } from '@angular/core';
import { MillisComponent } from './millis/millis.component';
import { FactoryResetComponent } from './factory-reset/factory-reset.component';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [MillisComponent, FactoryResetComponent],
  template: `
    <h1>Hello, {{ title }}</h1>
    <p>Congratulations! Your app is running. 🎉</p>
    <app-millis [model]="model$ | async" (reset)="millisService.reset()">System uptime unavailable</app-millis>
    <app-factory-reset>Factory reset unavailable</app-factory-reset>
  `
})

export class AppComponent {
  title = 'dmx-web-app';
  model$ = this.millisService.millis$

  constructor(private millisService: MillisService) {}
}
