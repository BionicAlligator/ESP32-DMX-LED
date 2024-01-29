import { ComponentFixture, TestBed } from '@angular/core/testing';

import { MillisComponent } from './millis.component';

describe('MillisComponent', () => {
  let component: MillisComponent;
  let fixture: ComponentFixture<MillisComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [MillisComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(MillisComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
