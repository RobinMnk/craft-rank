import express from 'express';
import { initialize } from 'express-openapi';
const app = express();

initialize({
    app,
    apiDoc: './src/endpoints.yml',
    paths: './paths'
});