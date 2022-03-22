function decorator() {
    var decorator = document.getElementById('decorator');

    setTimeout(function () {
        decorator.style.top = '-0%';

        setTimeout(function () {
            decorator.style.top = '-90%';
        }, 800);

        setTimeout(function () {
            decorator.style.top = '-35%';
        }, 1500);

        setTimeout(function () {
            decorator.style.top = '-74%';
        }, 2250);

        setTimeout(function () {
            decorator.style.top = '-40%';
        }, 2850);

        setTimeout(function () {
            decorator.style.top = '-50%';
        }, 3600);
    }, 0);
}
