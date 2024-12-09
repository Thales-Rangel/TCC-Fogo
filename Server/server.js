const express = require('express');
const mysql2 = require('mysql2');
const cors = require('cors');

const app = express();
app.use(cors());

// Configuração do banco de dados
const db = mysql.createPool({
    host: 'localhost',
    user: 'root',
    password: 'tcc-fogo',
    database: 'tcc_fogo'
});

app.get('/dados', (req, res) => {
    const query = 'SELECT * FROM registros ORDER BY id DESC';
    db.query(query, (err, results) => {
        if (err) {
            console.error(err);
            res.status(500).send('Erro no servidor');
        } else {
            res.json(results);
        }
    });
});

const PORT = 3000;
app.listen(PORT, () => {
    console.log(`Servidor rodando na porta ${PORT}`);
});