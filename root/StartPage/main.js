const image = ['bg.jpg', 'bg2.png'];
const rand = getRandomInt(2);

const body = document.body;

body.style.backgroundImage = `url(${image[rand]})`;

const signs = document.getElementsByClassName("signs");
const links = document.getElementsByTagName("a");
const fonts = document.getElementsByTagName("h1");

if (rand === 1){
    for (let link of links) {
        link.style.color = "aqua";
    }
    for (let sign of signs) {
        sign.style.color = "navajowhite";
    }
    for (let font of fonts) {
    }
}
else {
    for (let link of links) {
        link.style.color = "yellow";
    }
    for (let sign of signs) {
        sign.style.color = "darkblue";
    }
    for (let font of fonts) {
    }
}

function getRandomInt(max) {
    return Math.floor(Math.random() * max);
}