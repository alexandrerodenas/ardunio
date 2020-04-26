import {Injectable} from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {Observable} from 'rxjs';

export type Humidity = number;

@Injectable({
  providedIn: 'root'
})
export class FlowerPotsRepository {

  private readonly humidityPerSensorsEndpoint = 'http://192.168.0.17/measures.json';

  constructor(private readonly http: HttpClient) {

  }

  public gethumidityPerSensors(): Observable<Humidity[]> {
    return this.http.get<Humidity[]>(this.humidityPerSensorsEndpoint);
  }
}
