function TabelaTheme() {
    let corpo = document.querySelector('body');
    let IconTheme = document.querySelector('#butto-theme');
    let tabela = document.querySelector('table');
    let paragrafo = document.querySelector('caption');
    let themeIcon = document.querySelector('#themeIcon');
  
    // Alternar entre os temas
    if (corpo.classList.contains('dark-theme')) {
      // Modo Claro
      corpo.classList.remove('dark-theme');
      corpo.classList.add('light-theme');
      tabela.classList.remove('dark-theme');
      tabela.classList.add('light-theme');
      IconTheme.src = 'Imgs/icon/sol-icon.png'; // Ícone do modo claro
      paragrafo.style.color = '#000';
      themeIcon.style.borderColor = '#ffffff';
    } else {
      // Modo Escuro
      corpo.classList.remove('light-theme');
      corpo.classList.add('dark-theme');
      tabela.classList.remove('light-theme');
      tabela.classList.add('dark-theme');
      IconTheme.src = 'Imgs/icon/lua-icon.png'; // Ícone do modo escuro
      paragrafo.style.color = '#ffff';
      themeIcon.style.borderColor = '#dc143c';
    }
  }

  async function carregarDados() {
    try {
        const response = await fetch('http://10.0.1.200:3000/dados');
        const dados = await response.json();
        const tabela = document.getElementById('tabela-dados');

        // Limpar tabela
        tabela.innerHTML = '';

        // Inserir dados na tabela
        dados.forEach(dado => {
            const linha = document.createElement('tr');
            linha.innerHTML = `
                <td>${dado.id}</td>
                <td>${dado.ip_modolo}</td>
                <td>${dado.valor_fogo}</td>
                <td>${dado.valor_gas}</td>
                <td>${dado.status}</td>
                <td>${new Date(dado.data_hora).toLocaleString()}</td>
            `;
            tabela.appendChild(linha);
        });
    } catch (error) {
        console.error('Erro ao carregar os dados:', error);
    }
}

// Atualizar tabela a cada 30 segundos //
setInterval(carregarDados, 30000);
carregarDados(); // Carregar dados ao abrir a página