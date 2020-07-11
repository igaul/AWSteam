import Component from '@glimmer/component';
import { action } from '@ember/object';

import MAIN_MAP_STYLE from 'healthy-life/data/map-style';

export default class MainMapComponent extends Component {
  mapStyle = MAIN_MAP_STYLE;

  lat = 45.520301;
  lng = -122.626636;
  zoom = 13;

  @action
  panTo(id) {
    console.log(arguments)
    console.log(this.locations[id])
  }

  @action
  onLoad({ map, publicAPI }) {
    this.map = map;
    console.log(map, publicAPI )
  }
}
