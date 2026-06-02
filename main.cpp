#define CPPHTTPLIB_NO_FILE_MMAP
#include "httplib.h"
#include <iostream>
// ... остальной код
int main() {
    httplib::Server svr;

    // 1. Раздача статики (картинок)
    svr.set_mount_point("/images", "./images");

    // 2. API Товаров
    svr.Get("/products", [](const httplib::Request &, httplib::Response &res) {
        std::string json = "["
            "{\"name\": \"Чай Матча\", \"price\": 8500, \"img\": \"/images/1.jfif\"},"
        "{\"name\": \"Шоколад\", \"price\": 4200, \"img\": \"/images/2.jfif\"},"
        "{\"name\": \"Венчик\", \"price\": 5500, \"img\": \"/images/3.webp\"},"
        "{\"name\": \"Керамическая чаша\", \"price\": 12000, \"img\": \"/images/4.jfif\"},"
        "{\"name\": \"Матча латте микс\", \"price\": 6500, \"img\": \"/images/5.jfif\"},"
        "{\"name\": \"Подарочный бокс\", \"price\": 18000, \"img\": \"/images/6.jfif\"},"
        "{\"name\": \"Бамбуковая ложка\", \"price\": 2500, \"img\": \"/images/7.jfif\"},"
        "{\"name\": \"Трюфели\", \"price\": 7200, \"img\": \"/images/8.jfif\"},"
        "{\"name\": \"Матча из Киото\", \"price\": 9500, \"img\": \"/images/9.jfif\"},"
        "{\"name\": \"Сито для чая\", \"price\": 3800, \"img\": \"/images/10.jfif\"},"
        "{\"name\": \"Термос\", \"price\": 11000, \"img\": \"/images/11.jfif\"},"
        "{\"name\": \"Подставка под венчик\", \"price\": 4500, \"img\": \"/images/12.jfif\"},"
        "{\"name\": \"Чайный набор\", \"price\": 25000, \"img\": \"/images/13.webp\"},"
        "{\"name\": \"Печенье матча\", \"price\": 3200, \"img\": \"/images/14.jfif\"},"
        "{\"name\": \"Мед с матчей\", \"price\": 5000, \"img\": \"/images/15.jfif\"},"
        "{\"name\": \"Кокосовое молоко\", \"price\": 2800, \"img\": \"/images/16.jfif\"}"
        "]";
        res.set_content(json, "application/json; charset=utf-8");
    });

    // 3. API Заказов
    svr.Post("/order", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "[LOG] Получен заказ: " << req.body << std::endl;
        res.set_content("{\"status\": \"success\"}", "application/json");
    });

    // 4. Главная страница
    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        std::string html = R"(
<!DOCTYPE html>
<html lang='ru'>
<head>
    <meta charset='UTF-8'>
    <script src='https://cdn.tailwindcss.com'></script>
    <title>Matcha Shop</title>
</head>
<body class='bg-gradient-to-br from-green-50 to-emerald-100 min-h-screen font-sans'>    
    <header class='sticky top-0 z-50 backdrop-blur-lg bg-white/80 p-6 shadow-sm flex justify-between items-center'>
        <h1 class='text-3xl font-black tracking-tighter text-gray-900'>MATCHA<span class='text-green-600'>.</span></h1>
        <button onclick='showCart()' class='flex items-center gap-2 bg-gray-900 text-white px-6 py-3 rounded-2xl hover:bg-green-700 transition'>
            Корзина (<span id='count'>0</span>)
        </button>
    </header>

    <div class='group bg-white/70 backdrop-blur-md p-5 rounded-3xl shadow-sm hover:shadow-2xl transition-all duration-300 border border-white/50'>
        <div id='list' class='grid grid-cols-1 md:grid-cols-4 gap-8'></div>
    </div>
    
    <div id='cart-modal' class='hidden fixed inset-0 z-50 bg-black/50 flex items-center justify-center'>
        <div class='bg-white p-8 rounded-2xl w-1/2'>
            <h2 class='text-2xl font-bold mb-4'>Ваш заказ</h2>
            <div id='cart-items' class='mb-4'></div>
            <p id='total' class='font-bold text-xl mb-4'></p>
            <button onclick='sendOrder()' class='bg-black text-white px-6 py-2 rounded-lg'>Оплатить</button>
            <button onclick='showCart()' class='ml-2 text-gray-500'>Закрыть</button>
        </div>
    </div>

    <script>
        let cart = [];
        let userName = prompt('Введите ваше имя:') || 'Гость';
        
        fetch('/products').then(r => r.json()).then(data => {
            const list = document.getElementById('list');
            list.innerHTML = ''; // Очистка перед заполнением
            data.forEach(p => {
                list.innerHTML += `<div class='group bg-white p-5 rounded-3xl shadow-sm hover:shadow-2xl transition-all duration-300 border border-gray-100'>
                    <div class='overflow-hidden rounded-2xl'>
                        <img src='${p.img}' class='w-full h-56 object-cover group-hover:scale-105 transition-transform duration-500'>
                    </div>
                    <h2 class='mt-4 text-lg font-semibold text-gray-800'>${p.name}</h2>
                    <p class='text-xl font-bold text-green-700 mt-1'>${p.price} ₸</p>
                    <button onclick='addToCart("${p.name}", ${p.price})' 
                            class='mt-5 w-full bg-gray-900 text-white py-3 rounded-xl hover:bg-green-600 transition-colors font-medium'>
                        Добавить
                    </button>
                </div>`;
            });
        });

        function addToCart(name, price) {
            cart.push({name, price});
            document.getElementById('count').innerText = cart.length;
        }

        function showCart() {
            document.getElementById('cart-modal').classList.toggle('hidden');
            document.getElementById('cart-items').innerHTML = cart.map(i => `<p>${i.name} - ${i.price} ₸</p>`).join('');
            document.getElementById('total').innerText = "Итого: " + cart.reduce((s, i) => s + i.price, 0) + " ₸";
        }

        function sendOrder() {
            fetch('/order', { method: 'POST', body: JSON.stringify({user: userName, items: cart}) }).then(() => alert('Заказ отправлен!'));
        }
    </script>
</body>
</html>)";
        res.set_content(html, "text/html; charset=utf-8");
    });

    std::cout << "Сервер запущен на http://localhost:8080" << std::endl;
    svr.listen("0.0.0.0", 8080);
    return 0;
}