import { Component, Input} from '@angular/core';
import { CommonModule } from '@angular/common';

@Component({
  selector: 'app-millis',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './millis.component.html',
})

export class MillisComponent {
  @Input() dmxNodeModel: any;
}
