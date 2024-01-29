import { Component } from '@angular/core';
import { RouterOutlet } from '@angular/router';
import { MillisComponent } from './millis/millis.component';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [RouterOutlet, MillisComponent],
  templateUrl: './app.component.html',
  styleUrl: './app.component.css'
})
export class AppComponent {
  title = 'dmx-web-app';
}
