import Polyline from 'ember-google-maps/components/g-map/polyline';
// import { get, set } from '@ember/object';
// import { watch } from 'ember-google-maps/utils/options-and-events';

/**
 * A wrapper for the google.maps.Polyline class.
 *
 * @class Polyline
 * @namespace GMap
 * @module ember-google-maps/components/g-map/polyline
 * @extends GMap.MapComponent
 */
export default Polyline.extend({
  _type: 'polygon',

  // _createOptions(options) {
  //   return {
  //     ...options,
  //     path: this.path,
  //     map: this.map,
  //   };
  // },

  _addComponent(options) {
    return Promise.resolve(
      this.set('mapComponent', new google.maps.Polygon(options))
    );
  },

  // _didAddComponent() {
  //   let watched = watch(this,
  //     { 'path.[]': () => this._updateOrAddComponent() }
  //   );

  //   watched
  //     .forEach(({ name, remove }) => this._eventListeners.set(name, remove));

  //   return this._super(...arguments);
  // },
});