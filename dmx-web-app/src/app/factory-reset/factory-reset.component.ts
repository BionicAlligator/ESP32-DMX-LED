import { Component, EventEmitter, Input, Output } from '@angular/core';
import { DmxNodeModel } from '../dmx-node-model';

@Component({
  selector: 'app-factory-reset',
  standalone: true,
  imports: [],
  templateUrl: './factory-reset.component.html'
})
export class FactoryResetComponent {
  @Input() dmxNodeModel2: any;

  onClick() {
    console.log("clicked");
    this.dmxNodeModel2.status.state="reset";
  }
}
