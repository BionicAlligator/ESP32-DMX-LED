import { Component } from '@angular/core';
import { ModelSynchroniser } from '../model-synchroniser';

@Component({
  selector: 'app-factory-reset',
  standalone: true,
  imports: [],
  templateUrl: './factory-reset.component.html'
})
export class FactoryResetComponent {
  constructor(private modelChangesFromClient: ModelSynchroniser) { }

  onClick() {
    console.log("Reset clicked");
    this.modelChangesFromClient.send({ status: { state: "reset" } });
  }
}
