import {AfterViewInit, Component, ElementRef, EventEmitter, OnInit, Output, ViewChild} from '@angular/core';
import {FlowerPotsRepository} from './flower-pots.repository';
import * as Highcharts from 'highcharts';
import * as HighchartsMore from 'highcharts/highcharts-more.src';
import * as HC_solide_gauge from 'highcharts/modules/solid-gauge.src';
import {chartOptions} from './chart-options';

HighchartsMore(Highcharts);
HC_solide_gauge(Highcharts);

@Component({
  selector: 'app-flower-pots',
  templateUrl: './flower-pots.component.html',
  styleUrls: ['./flower-pots.component.css']
})
export class FlowerPotsComponent implements OnInit, AfterViewInit {

  @Output() loading = new EventEmitter<boolean>(false);


  private readonly updateInterval = 10000;

  @ViewChild('chartA')
  private readonly chartARef: ElementRef;
  @ViewChild('chartB')
  private readonly chartBRef: ElementRef;
  @ViewChild('chartC')
  private readonly chartCRef: ElementRef;

  private chartA: any;
  private chartB: any;
  private chartC: any;

  constructor(private readonly flowerPotsRepository: FlowerPotsRepository) {
  }

  private static createChart(el, cfg): any {
    return Highcharts.chart(el, cfg);
  }

  ngOnInit(): void {
    setInterval(() => this.refreshHumidityValues(), this.updateInterval);
  }

  ngAfterViewInit(): void {
    this.chartA = FlowerPotsComponent.createChart(this.chartARef.nativeElement, chartOptions(0));
    this.chartB = FlowerPotsComponent.createChart(this.chartBRef.nativeElement, chartOptions(0));
    this.chartC = FlowerPotsComponent.createChart(this.chartCRef.nativeElement, chartOptions(0));
  }


  private refreshHumidityValues(): void {
    this.loading.emit(true);
    this.flowerPotsRepository.gethumidityPerSensors().subscribe(
      humidityRates => {
        this.chartA = FlowerPotsComponent.createChart(this.chartARef.nativeElement, chartOptions(humidityRates[0]));
        this.chartB = FlowerPotsComponent.createChart(this.chartBRef.nativeElement, chartOptions(humidityRates[1]));
        this.chartC = FlowerPotsComponent.createChart(this.chartCRef.nativeElement, chartOptions(humidityRates[2]));
        this.loading.emit(false);
      }
    );
  }
}
