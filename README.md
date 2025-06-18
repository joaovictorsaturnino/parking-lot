
# Estacionamento com ESP32 e Sensores Ultrassônicos

Este projeto utiliza um ESP32, sensores ultrassônicos e um display LCD I2C para monitorar o número de vagas livres e ocupadas em um estacionamento.

## 🚀 Funcionalidades

- Detecta a presença de veículos em até 3 vagas.
- Exibe no display LCD:
  - Número de vagas livres.
  - Número de vagas ocupadas.
- Atualização automática e contínua dos dados.

## 🛠️ Componentes usados

- ESP32 DevKit
- 3 sensores ultrassônicos (HC-SR04)
- Display LCD 16x2 com interface I2C
- Jumpers, protoboard (se aplicável)

## 📦 Estrutura do projeto

```

esp32-estacionamento/
├── src/
│   └── main.cpp
├── platformio.ini
└── README.md

````

## ⚙️ Configuração do PlatformIO

### `platformio.ini`

```ini
[env:esp32dev]
platform = espressif32
framework = arduino
board = esp32dev

lib_deps =
  marcoschwartz/LiquidCrystal_I2C@^1.1.4
````

## 💡 Como usar

1. Instale o [PlatformIO](https://platformio.org/) no VS Code.
2. Conecte seu ESP32 via USB.
3. Compile e envie o código para o ESP32.
4. Acompanhe a exibição no LCD.

## 🔌 Esquema de ligação (resumo)

| Sensor   | TRIG Pin | ECHO Pin |
| -------- | -------- | -------- |
| Sensor 1 | GPIO 12  | GPIO 13  |
| Sensor 2 | GPIO 14  | GPIO 27  |
| Sensor 3 | GPIO 26  | GPIO 25  |

> O display LCD utiliza conexão I2C nos pinos padrão do ESP32 (GPIOS 21 e 22).

## 📖 Licença

Este projeto está licenciado sob a [MIT License](LICENSE) – sinta-se livre para usar e modificar.

---

### ✍️ Autor

Desenvolvido por João Victor da Silva Saturnino – sinta-se à vontade para contribuir!

