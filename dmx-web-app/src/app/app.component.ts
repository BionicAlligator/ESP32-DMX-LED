import { Component } from '@angular/core';
import { MillisComponent } from './millis/millis.component';
import { FactoryResetComponent } from './factory-reset/factory-reset.component';
import { CommonModule } from '@angular/common';
import { ModelSynchroniser } from './model-synchroniser';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [MillisComponent, FactoryResetComponent, CommonModule],
  templateUrl: './app.component.html'
})

export class AppComponent {
  title = 'dmx-web-app';
  modelChange$ = this.modelSynchroniser.modelChange$;
  constructor(private modelSynchroniser: ModelSynchroniser) {}
}
