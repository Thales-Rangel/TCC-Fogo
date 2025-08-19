import express from 'express';
import cors from 'cors';
import mysql from 'mysql2';

const app = express();
app.use(cors());
app.use(express.json());

const db = mysql.createConnection({
    host: 'localhost',
    user: 'root',
    password: 'admin',
    database: 'tcc_fogo'
});

// Get dos registros dos módulos
app.get('/dados', (req, res) => {
    const query = 'SELECT * FROM registros ORDER BY id DESC LIMIT 30';

    db.query(query, (err, results) => {
        if (err) throw err;

        res.status(200).json(results);
    });
});

// Post de módulos
app.post('/modulos', (req, res) => {
    const sql = `INSERT INTO modolos VALUES 
                ("${req.body.end_ip}", "${req.body.ref}", "${req.body.num_sala}", ${req.body.limt_temp}, ${req.body.limt_gas}, "${req.body.end_mac}")`;

    db.execute(sql, (err, results) => {
        if (err) throw err;

        res.status(201);
    });

    res.json(req.body);
});

// Get de módulos
app.get('/modulos', (req, res) => {
    const sql = `SELECT * FROM modolos ORDER BY end_ip`;

    db.query(sql, (err, results) => {
        if (err) throw err;

        res.status(200).json(results);
    });
});

// Put de módulos
app.put('/modulos/:end_ip', (req, res) => {
    const sql = `UPDATE modolos 
                SET ref="${req.body.ref}", num_sala="${req.body.num_sala}", limt_temp=${req.body.limt_temp}, limt_gas="${req.body.limt_gas}" 
                WHERE end_ip="${req.params.end_ip}"`;

    db.execute(sql, (err, results) => {
        if (err) throw err;

        res.status(200).json(`${results.affectedRows} módulos modificados!`);
    });
});

// Delete de módulos
app.delete('/modulos/:end_ip', (req, res) => {
    const sql = `DELETE FROM modolos WHERE end_ip="${req.params.end_ip}"`;

    db.execute(sql, (err, results) => {
        if (err) throw err;

        res.status(200).json(`${results.affectedRows} módulos deletados!`);
    })
});


app.listen(3000, () => {
    console.log('Servidor rodando na porta 3000');
});