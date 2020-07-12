import Component from '@glimmer/component';
import { action, set } from '@ember/object';
import { tracked } from '@glimmer/tracking';
import { sort } from '@ember/object/computed';

import MAIN_MAP_STYLE from 'healthy-life/data/map-style';


function generateBounds(points) {
  const bounds = new google.maps.LatLngBounds();
  
  points.forEach(point => bounds.extend(new google.maps.LatLng(point)));

  return bounds;
}


export default class MainMapComponent extends Component {
  mapStyle = MAIN_MAP_STYLE;

  lat = 45.520301;
  lng = -122.626636;
  @tracked zoom = 13;

  sortedDensity

  densitySorting = ['density.number', 'name'];
  @sort('args.locations', 'densitySorting') sortedLocations;

  @action
  panTo(id) {
    const location = this.args.locations.find(location => location.id === id);
    const previousLocation = this.args.locations.find(location => location.activeOutline === 1)
    const bounds = generateBounds(location.shapes[0]);

    if (previousLocation) {
      set(previousLocation, 'activeOutline', 0);
    }
    set(location, 'activeOutline', 1);

    google.maps.event.addListenerOnce(this.map, "idle", () => { 
      google.maps.event.addListenerOnce(this.map, "idle", () => { 
        this.map.setZoom(14);
      });
      this.map.panTo(bounds.getCenter());
    });
    this.map.setZoom(13);
  }

  @action
  onLoad({ map }) {
    this.map = map;
  }
}
