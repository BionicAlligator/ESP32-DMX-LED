import { Component } from '@angular/core';
import { MillisComponent } from './millis/millis.component';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [MillisComponent],
  templateUrl: './app.component.html'
})

export class AppComponent {
  title = 'dmx-web-app';
}
