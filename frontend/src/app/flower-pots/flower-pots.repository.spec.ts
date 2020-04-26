import { TestBed } from '@angular/core/testing';

import { FlowerPotsRepository } from './flower-pots.repository';

describe('FlowerPotsRepositoryService', () => {
  let service: FlowerPotsRepository;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(FlowerPotsRepository);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
