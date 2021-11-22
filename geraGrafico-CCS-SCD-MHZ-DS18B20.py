#!/usr/bin/env python
# coding: utf-8

'''
Lê arquivos com dados dos sensores CCS811, SCD30, MHZ19C e DS18B20 e 
gera gráfico com Plotly
Gráfico é gravado em um arquivo .html que tem a data como nome
Dados dos sensores gravados via serial logger em um arquivo csv 
com timestamp no formato ISO 8601 na primeira coluna

Nome dos arquivos segue padrão "serial-DATA-NOMESENSOR.log"
onde
DATA: no formato AAAA-MM-DD
NOMESENSOR: CCS811, SCD30, MHZ19, DS18B20 (este último só temperatura)

Como executar? Exemplo:
python geraGrafico-CCS-SCD-MHZ-DS18B20.py 2021-11-09 dados docs "\t"

Fernando S. Pacheco
Novembro-2021
'''

import pandas as pd
import plotly.express as px

import sys

if __name__ == '__main__':
    if len(sys.argv) < 5:
        print('Uso: ', sys.argv[0], ' data pasta_entrada pasta_saida separador')
        print('    Por exemplo: ', sys.argv[0], ' 2021-11-11 dados docs "\\t"')
        exit(1)
    data = sys.argv[1]
    pasta_entrada = sys.argv[2]
    pasta_saida = sys.argv[3]
    separador = sys.argv[4]

# Leitura de arquivos
# CCS #######################
dfCCS = pd.read_csv(pasta_entrada+'/'+'serial-'+data+'-CCS811.log', 
                    delimiter=separador, 
                    names=['Data','CCS1-CO2ppm','CCS1-VOCppb','CCS2-CO2ppm','CCS2-VOCppb','SensorName'],
                    parse_dates=[1],
                    engine='python')
dfCCS['Data'] = pd.to_datetime(dfCCS['Data'])
# não sei porque não converteu a primeira coluna de dados para numérico, mas agora vai fazer:
dfCCS['CCS1-CO2ppm'] = pd.to_numeric(dfCCS['CCS1-CO2ppm'])
dfCCS.info()

# SCD #######################
dfSCD = pd.read_csv(pasta_entrada+'/'+'serial-'+data+'-SCD30.log', 
                    delimiter='\t', 
                    names=['Data','SCD30-CO2ppm','SCD30-TempCelsius','SCD30-Umid%','SensorName'],
                    parse_dates=[1],
                    engine='python')
dfSCD['Data'] = pd.to_datetime(dfSCD['Data'])
dfSCD['SCD30-CO2ppm'] = pd.to_numeric(dfSCD['SCD30-CO2ppm'])

# tem alguns valores com leitura errada. Serão excluídos
dfSCD = dfSCD[dfSCD['SCD30-CO2ppm']>-2147483648]
dfSCD = dfSCD[dfSCD['SCD30-CO2ppm']!=0]
dfSCD.info()

# MHZ #######################
dfMHZ = pd.read_csv(pasta_entrada+'/'+'serial-'+data+'-MHZ19.log', 
                    delimiter='\t', 
                    names=['Data','MHZ1-CO2ppm-Serial','MHZ1-CO2ppm-PWM', 'MHZ1-TempCelsius','MHZ2-CO2ppm-Serial','MHZ2-CO2ppm-PWM', 'MHZ2-TempCelsius','SensorName'],
                    parse_dates=[1],
                    engine='python')
dfMHZ['Data'] = pd.to_datetime(dfMHZ['Data'])
dfMHZ['MHZ1-CO2ppm-Serial'] = pd.to_numeric(dfMHZ['MHZ1-CO2ppm-Serial'])
dfMHZ.info()

# DS18B20 #######################
dfDS18B20 = pd.read_csv(pasta_entrada+'/'+'serial-'+data+'-DS18B20.log', 
                    delimiter='\t', 
                    names=['Data','DS18B20-Celsius','SensorName'],
                    parse_dates=[1],
                    engine='python')
dfDS18B20['Data'] = pd.to_datetime(dfDS18B20['Data'])
dfDS18B20['DS18B20-Celsius'] = pd.to_numeric(dfDS18B20['DS18B20-Celsius'])
dfDS18B20.info()

# TODO: Verificar se merge_asof é a melhor solução
dfTotal = pd.merge_asof(dfSCD, dfCCS, on = 'Data')
dfTotal = pd.merge_asof(dfTotal, dfMHZ, on = 'Data')
dfTotal = pd.merge_asof(dfTotal, dfDS18B20, on = 'Data')

fig = px.line(dfTotal, x='Data', 
    y=['CCS1-CO2ppm','CCS2-CO2ppm','SCD30-CO2ppm','MHZ1-CO2ppm-PWM','MHZ2-CO2ppm-PWM','DS18B20-Celsius'])
fig.show()

import plotly.io as pio
pio.write_html(fig, file=pasta_saida+'/'+data+'.html', auto_open=True)

# Interessante ver:
# https://pandas.pydata.org/pandas-docs/dev/getting_started/intro_tutorials/09_timeseries.html
# https://pandas.pydata.org/pandas-docs/dev/getting_started/intro_tutorials/08_combine_dataframes.html
