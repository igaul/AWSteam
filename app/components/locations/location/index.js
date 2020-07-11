import Component from '@glimmer/component';
import { htmlSafe } from '@ember/template';

export default class LocationsLocationComponent extends Component {
  styleNamespace = '__locations__location';

  get color() {
    return htmlSafe(`color: ${this.args.location.density.color}`);
  }
}
