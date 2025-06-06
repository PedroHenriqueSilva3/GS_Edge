🌊 HidroSafe - Não é sorte, é prevenção inteligente.

📌 Descrição do Problema
O Brasil enfrenta desafios graves com enchentes, que geram impactos sociais, econômicos e humanos. Segundo o Banco Mundial, o país perde anualmente cerca de R$ 8 bilhões com desastres naturais, sendo as enchentes responsáveis por mais de 60% desse valor — o equivalente a R$ 4,8 bilhões. Além dos danos materiais, há perdas humanas significativas: entre 2001 e maio de 2024, 3.522 pessoas morreram em enchentes no Brasil, com 224 mortes apenas em 2024, segundo o site Valor Econômico. As enchentes urbanas recorrentes causam perdas materiais, interrupções de serviços essenciais e colocam vidas em risco, especialmente em áreas residenciais mal preparadas. A falta de infraestrutura adequada expõe famílias à insegurança, instabilidade e prejuízos financeiros em períodos de chuva intensa.

✅ Visão Geral da Solução
Desenvolvemos uma plataforma web inteligente e solidária que oferece suporte em tempo real para pessoas afetadas por enchentes. O sistema combina sensores físicos, análise de dados, geolocalização e funcionalidades sociais para oferecer ajuda direta, previsões e informações essenciais. Entre as principais funcionalidades estão: (1) Localização de Abrigos Seguros — com mapa interativo, capacidade atualizada, horários e rotas de acesso; (2) Dicas de Sobrevivência — orientações práticas sobre o que fazer antes, durante e depois das enchentes, além de primeiros socorros; (3) Ajuda Solidária — sistema de doações de alimentos, roupas, água, itens de higiene e dinheiro, conectando doadores a instituições confiáveis; (4) Monitoramento Ambiental em Tempo Real — sensores locais enviam dados via WebSocket para um backend em Python, que usa modelos preditivos para avaliar risco de enchentes e deslizamentos com base geográfica; (5) Painel Informativo Personalizado — identifica a localização do usuário e exibe alertas específicos da sua região, com interface acessível e intuitiva para todas as idades.

▶️ Como simular o projeto
1. Acesse o projeto no Wokwi pelo link abaixo.
https://wokwi.com/projects/432953183866740737

2. Clique em "Start Simulation" no canto superior direito.

3. Observe os sensores conectados ao ESP32:

* DHT22: simula leitura de temperatura e umidade.

* HC-SR04: simula o nível da água com base na distância medida.

* MPU6050: simula velocidade do vento usando valor do X, usado devido a falta de um anenometro.

4. O código no ESP32 processa esses dados e exibe as leituras no terminal serial.

5. Você pode alterar os valores dos sensores diretamente na simulação para testar diferentes cenários, como aumento do nível da água ou variações de temperatura.

▶️ Link do vídeo demonstrativo
https://youtu.be/-GZzqPOE7MI