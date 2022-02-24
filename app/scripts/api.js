const app = require('express')();
const PORT = 8888;

app.listen(
    PORT,
    () => console.log('API on http://localhost:', PORT)
);

app.get('/bpm', (req, res) => {
    res.status(202).send({
        mode: "test"
    })
})

app.put('/bpm', (req, res) => {
    res.status(202).send({
        mode: "test"
    })
})

app.get('/bpm/min', (req, res) => {
    res.status(202).send({
        mode: "test"
    })
})

app.delete('/bpm/min', (req, res) => {
    res.status(202).send({
        mode: "test"
    })
})

app.get('/bpm/max', (req, res) => {
    res.status(202).send({
        mode: "test"
    })
})

app.delete('/bpm/max', (req, res) => {
    res.status(202).send({
        mode: "test"
    })
})