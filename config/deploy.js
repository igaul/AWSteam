/* eslint-env node */
'use strict';

module.exports = function() {
  return {
    build: {
      environment: 'production',
    },
    pipeline: {
      activateOnDeploy: true
    },
    s3: {
      bucket: process.env.DEPLOY_BUCKET,
      region: process.env.DEPLOY_REGION,
      accessKeyId: process.env.AWS_KEY,
      secretAccessKey: process.env.AWS_SECRET,
      filePattern: '*',
    },
    "s3-index": {
      bucket: process.env.DEPLOY_BUCKET,
      region: process.env.DEPLOY_REGION,
      accessKeyId: process.env.AWS_KEY,
      secretAccessKey: process.env.AWS_SECRET,
      allowOverwrite: true
    },
    cloudfront: {
      accessKeyId: process.env.AWS_KEY,
      secretAccessKey: process.env.AWS_SECRET,
      distribution: process.env.DEPLOY_DISTRIBUTION,
    }
  };
};
