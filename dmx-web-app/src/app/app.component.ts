import { Component } from '@angular/core';
import { MillisComponent } from './millis/millis.component';
import { FactoryResetComponent } from './factory-reset/factory-reset.component';
import { CommonModule } from '@angular/common';
import { ModelSynchroniser } from './model-synchroniser';
import { DmxUniverseComponent } from './dmx-universe/dmx-universe.component';

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [MillisComponent, FactoryResetComponent, DmxUniverseComponent, CommonModule],
  templateUrl: './app.component.html'
})

export class AppComponent {
  title = 'dmx-web-app';
  modelChangesFromServer$ = this.modelSynchroniser.modelChangesFromServer$;

  constructor(private modelSynchroniser: ModelSynchroniser) { }
}
