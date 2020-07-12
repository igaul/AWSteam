import Component from '@glimmer/component';
import { htmlSafe } from '@ember/template';
import { computed } from '@ember/object';

export default class LocationsLocationComponent extends Component {
  styleNamespace = '__locations__location';

  @computed('args.location.density.color')
  get color() {
    return htmlSafe(`color: ${this.args.location.density.color}`);
  }

  @computed('args.location.{density.color,activeOutline}')
  get cellColor() {
    const newBackground = this.args.location.activeOutline ? this.args.location.density.color : 'black';
    return htmlSafe(`color: ${newBackground}`);
  }
}
