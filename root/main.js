const image = ['bg.jpg', 'bg2.png'];
const rand = getRandomInt(2);

const body = document.body;

console.log(image[rand]);

body.style.backgroundImage = `url(${image[rand]})`;

function getRandomInt(max) {
    return Math.floor(Math.random() * max);
}