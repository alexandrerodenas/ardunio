import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { FlowerPotsComponent } from './flower-pots.component';

describe('FlowerPotsComponent', () => {
  let component: FlowerPotsComponent;
  let fixture: ComponentFixture<FlowerPotsComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ FlowerPotsComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(FlowerPotsComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
