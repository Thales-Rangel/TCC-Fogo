function aparecer() {
    let inputPass = document.getElementById('senha')
    let btn = document.getElementById('btn-senha')

    if(inputPass.type === 'password') {
        inputPass.setAttribute('type', 'text')

        btn.classList.replace('bi-eye-fill', 'bi-eye-slash-fill')
    }
//Colocar novamente em modo oculto
    else {
        inputPass.setAttribute('type', 'password')

        btn.classList.replace('bi-eye-slash-fill', 'bi-eye-fill')
    }
}