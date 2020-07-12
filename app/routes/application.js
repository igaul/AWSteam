import Route from '@ember/routing/route';

import geoLocations from 'healthy-life/data/parks';

function formatCordinates(shape) {
  return shape.map(function([lng, lat]) {
    return { lat, lng, };
  });
}

function getShapes(coordinates) {
  return coordinates.map(function(shapes) {
    if (!Array.isArray(shapes[0][0])) {
      shapes = [shapes];
    }
    return shapes.map(formatCordinates)[0];
  })
}

function getRandomIntRange(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

const densityColors = [
  '#0CE500',
  '#3CE000',
  '#6ADC00',
  '#97D800',
  '#C1D400',
  '#CFB500',
  '#CB8500',
  '#C75700',
  '#C32C00',
  '#BF0100',
];


const locations = geoLocations.features.map(feature => {
  const density = getRandomIntRange(1, 10);
  const shapes = getShapes(feature.geometry.coordinates)
  return {
    shapes: shapes,
    id: feature.properties.OBJECTID,
    name: feature.properties.NAME,
    activeOutline: 0,
    density: {
      number: density,
      color: densityColors[density - 1],
    }
  }
});

export default class ApplicationRoute extends Route {
  async model() {
    return locations;
  }
}
