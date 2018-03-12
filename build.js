const glob = require('glob');
const xmldom = require('xmldom-qsa');
const fs = require('fs');

const project = fs.readFileSync('FreeFlow.jucer', 'utf8');

const parser = new xmldom.DOMParser();
const doc = global.document = parser.parseFromString(project);

const el = doc.querySelector('MAINGROUP > GROUP');
el.childNodes = [];

const serializer = new xmldom.XMLSerializer();
const tmp = serializer.serializeToString(doc);

glob.sync('Source/**/*.*').forEach(file => {
   console.log(file);

});

