const express = require('express');
const mysql = require('mysql2/promise');
const hirestime = require('hirestime')

'use strict';

// Параметры web-сервера
const server_host = 'localhost';
const server_port = '3000';

// Параметры подключения к MySQL
const conn = { host: 'localhost', port: 3306, database: 'uir5', user: 'root', password : '9791' };

// Номер id для оценки времени
const test_id = 100;

// Тестовые запросы
const queries = [
    { table: 'articles_no_part', sql: `SELECT SQL_NO_CACHE * FROM articles_no_part LIMIT 1` },
    { table: 'articles_no_part', sql: `SELECT SQL_NO_CACHE * FROM articles_no_part WHERE id = ${test_id} LIMIT 1` },
    { table: 'articles_part',    sql: `SELECT SQL_NO_CACHE * FROM articles_part LIMIT 1` },
    { table: 'articles_part',    sql: `SELECT SQL_NO_CACHE * FROM articles_part WHERE id = ${test_id} LIMIT 1` },
];

async function run() {
    // Установить соединение с сервером
    const db = await mysql.createConnection(conn);

    // Создать web сервис
    const app = express();

    // Использовать pug как HTML template движок
    app.set('view engine', 'pug');

    // По запросу http://server:port/ выполнить 4 тестовых выборки из двух таблиц
    app.get('/', wrapAsync(async (req, res, next) => {
    // Получить количество элементов в таблице (только для отчета)
    const [ rows ] = await db.execute(`SELECT COUNT(*) AS n FROM ${queries[0].table}`);
    const n = rows[0].n;

    // Выполнить тестовые выборки
    let results = [];
    for (let i in queries) {
        let time = hirestime();
        let [ rows, fields ] = await db.execute(queries[i].sql);
        let elapsed = time(hirestime.MS).toFixed(2);

        results.push({
            n: n,
            sql: queries[i].sql,
            table: queries[i].table,
            time: elapsed,
            rows: rows,
            fields: fields
        });
    }

    // Отобразить страницу результатов
    res.render('results', { header: `Query results`, results: results });
}));

    // Запустить HTTP сервер
    app.listen(server_port, server_host, () => {
        console.log(`Server running at http://${server_host}:${server_port}/, press Ctrl-C to exit`);
    });
}

// Функция для обработки ошибок
function wrapAsync(fn) {
    return function(req, res, next) {
        fn(req, res, next).catch(next);
    };
}

run();
