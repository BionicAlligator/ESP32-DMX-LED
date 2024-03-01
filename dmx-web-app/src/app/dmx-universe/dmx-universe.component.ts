import { Component, Input } from '@angular/core';
import { CommonModule } from '@angular/common';
import { ModelSynchroniser } from '../model-synchroniser';

@Component({
  selector: 'app-dmx-universe',
  standalone: true,
  imports: [CommonModule],
  template: `<p>DMX Universe: <input #dmx_universe value="{{dmxNodeModel.status.uptime_millis}}" (keyup.enter)="setUniverse(dmx_universe.value)" (blur)="setUniverse(dmx_universe.value); dmx_universe.value = ''" /></p>`
})

export class DmxUniverseComponent {
  @Input() dmxNodeModel: any;

  //TODO: This is modelSynchroniser, not modelChangesFromClient  (change also in factory-reset component)
  constructor(private modelChangesFromClient: ModelSynchroniser) { }

  setUniverse(universe: string) {
    console.log("Setting DMX Universe to " + universe)

    //TODO: This method should probably be called sendModelChangesFromClient or updateServer
    this.modelChangesFromClient.send({ config: { dmx_universe: universe } });
  }
}
