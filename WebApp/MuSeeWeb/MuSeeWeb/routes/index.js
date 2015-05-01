var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function (req, res) {
    res.render('index', { title: 'MuSee' });
});

router.get('/dev/', function (req, res) {
    res.render('dev', { title: 'MuSee' });
});

router.get('/jsrender/', function (req, res) {
    res.render('jsrender', { title: 'MuSee' });
});

module.exports = router;