import express from 'express';
import cors from 'cors';
import mysql from 'mysql2';

const app = express();
app.use(cors());
app.use(express.json());

const db = mysql.createConnection({
    host: 'localhost',
    user: 'root',
    password: 'tcc-fogo',
    database: 'tcc_fogo'
});

app.get('/dados', (req, res) => {
    const query = 'SELECT * FROM registros';

    db.query(query, (err, results) => {
        if (err) throw err;

        res.status(200).json(results);
    });
});

app.listen(3000, () => {
    console.log('Servidor rodando na porta 3000');
});