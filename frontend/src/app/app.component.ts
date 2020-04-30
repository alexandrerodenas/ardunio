import {Component} from '@angular/core';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'HomeWatcher';
  enableProgressBar = false;

  handleLoadingState($event: boolean) {
    this.enableProgressBar = $event;
  }
}
