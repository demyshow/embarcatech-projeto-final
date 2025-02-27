# 🎮 Projeto: Fuga Explosiva

## 📌 Unidade 7 - Capítulo 1: Projeto de Sistema Embarcado

### 🎯 Objetivos Realizados
- Foi criado um projeto original e inovador.
- O projeto foi desenvolvido de forma autoral, sem ser uma cópia de outro existente.
- O desenvolvimento foi baseado nos conceitos de sistemas embarcados aprendidos durante o curso.
- A plataforma BitDogLab foi utilizada exclusivamente para representar o projeto.
- O projeto foi de minha autoria
- Foi baseado nos conceitos de sistemas embarcados aprendidos durante o curso.
- Foi usada plataforma BitDogLab para representar o projeto.

### 📌 Definição e Documentação do Projeto
- **Escopo**: A função do sistema embarcado foi claramente definida.
- **Pesquisa**: Foram analisados projetos semelhantes para garantir a originalidade.
- **Especificações de Hardware e Software**: Todos os componentes foram listados e detalhados em sua aplicação.
- **Implementação**: O projeto foi desenvolvido e testado com sucesso.
- **Documentação**: Foi criado um relatório detalhado contendo todas as informações do projeto.
- **Pesquisa**: Buscar projetos semelhantes para garantir a originalidade.
- **Especificações de Hardware e Software**: Listar os componentes e explicar como serão usados.
- **Implementação**: Desenvolver o projeto e realizar testes.
- **Documentação**: Criar um relatório detalhado com todas as informações do projeto.

### ✅ O que foi feito
- Desenvolvido um jogo interativo embarcado utilizando a placa **BitDogLab**.
- Implementado um sistema de controle por **joystick analógico**.
- Exibidas informações e pontuação em um **display OLED SSD1306**.
- Integrados **LEDs RGB e buzzer** para feedback visual e sonoro.
- Implementar um sistema de controle por **joystick analógico**.
- Exibir informações e pontuação em um **display OLED SSD1306**.
- Integrar **LEDs RGB e buzzer** para feedback visual e sonoro.

---

## 🛠️ Descrição do Projeto
O jogo **Fuga Explosiva** consiste em controlar um personagem na tela que deve evitar colisões com inimigos. O jogador movimenta o personagem utilizando um **joystick**, e a dificuldade aumenta conforme a pontuação sobe.

### 📟 Controle do Display OLED
- Exibição do personagem e dos inimigos na tela OLED.
- Exibição de **pontuação e velocidade dos inimigos**.
- Indicação visual de **jogo pausado** ou **finalizado**.

### 💡 Controle dos LEDs RGB e Buzzer
- **LED Azul**: Indica que o jogo está em andamento.
- **LED Verde**: Acende quando o jogador vence.
- **LED Vermelho**: Indica derrota por colisão.
- **Buzzer**:
  - Som curto ao vencer.
  - Som de alerta ao perder.

### 🔘 Botões e suas funcionalidades
- **Botão do Joystick (GPIO 22)**: Pausa e retoma o jogo.
- **Botão A (GPIO 5)**: Liga ou desliga o jogo.
- **Botão B (GPIO 6)**: Reinicia o jogo.

---

## ✅ Requisitos do Projeto
- **Utilização do protocolo I2C** para comunicação com o **display OLED**.
- **Organização modular do código** para facilitar a leitura e manutenção.

---

## 🖥️ Configuração do Ambiente

### 📦 Instalações Necessárias
- **Compilador TDM-GCC para Raspberry Pi Pico**.
- **CMake para gerenciamento de build**.
- **Git para controle de versão**.
- **GitHub para hospedagem do código**.
- **Placa RP2040 BitDogLab conectada via USB**.
- **Pico SDK configurado**.
- **GNU ARM Toolchain instalado**.
- **Visual Studio Code com extensões**:
  - C/C++
  - CMake Tools
  - Raspberry Pi Pico
  - Wokwi Simulator

---

## 🔌 Conexões de Hardware

| Componente | Pino GPIO |
|------------|----------|
| **I2C SDA** | 14 |
| **I2C SCL** | 15 |
| **Joystick X** | 27 |
| **Joystick Y** | 26 |
| **Botão Joystick** | 22 |
| **Botão A (Start)** | 5 |
| **Botão B (Reset)** | 6 |
| **LED Verde (PWM)** | 11 |
| **LED Azul (PWM)** | 12 |
| **LED Vermelho (PWM)** | 13 |
| **Buzzer** | 21 |

---

## 🗁 Estrutura do Código

📂 `C:\embarcatech-projeto-final\embarcatech-projeto-final\`
```
│── 📄 embarcatech-projeto-final.c  # Código principal
│── 📄 CMakeLists.txt   # Configuração para compilação no Pico SDK
│── 📂 lib/             # Biblioteca SSD1306 e fontes para o display OLED
│── 📄 wokwi.toml       # Arquivo de configuração para o simulador Wokwi
│── 📄 README.md        # Documentação do projeto
```

---

## 📸 Demonstração e Entrega

### 📝 Código-fonte
O código do projeto deve ser enviado em um **repositório GitHub**, contendo todos os arquivos necessários para sua execução.

🔗 **Repositório GitHub**: [https://github.com/demyshow/embarcatech-projeto-final](#)

### 🎥 Vídeo de demonstração
O vídeo deve ter até **2 minutos**, mostrando:
- O aluno demonstrando o funcionamento do projeto.
- Breve explicação das funcionalidades implementadas.
- Execução do projeto na **placa BitDogLab**.

📹 **Assista à demonstração do projeto**: [https://drive.google.com/drive/u/2/folders/1kkv8uVrtj4gIV5LIVsnWvzAqXxLd1998] (#)

---

## 📦 Como Clonar e Executar o Projeto
```sh
git clone https://github.com/demyshow/embarcatech-projeto-final.git
cd C:\embarcatech-projeto-final\embarcatech-projeto-final
```
Compile e execute o código usando o **Pico SDK**.

Após clonar o projeto, ele deve ser importado no **Raspberry Pi** via o seguinte caminho:
```
C:\embarcatech-projeto-final\embarcatech-projeto-final
```

---

## 🚀 Melhorias Futuras
- Implementação de **níveis de dificuldade**.
- Adicionar **sprites gráficos** ao jogo.
- Criar **efeitos sonoros personalizados**.

---

## 📌 Autor
👤 **Edemir de Carvalho Rodrigues**
