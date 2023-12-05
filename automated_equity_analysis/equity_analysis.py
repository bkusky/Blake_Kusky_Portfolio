#!/usr/bin/env python3

import requests
import pandas as pd
import numpy as np
import datetime
from io import BytesIO
import sys
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
import xgboost as xgb
from sklearn.metrics import accuracy_score, classification_report
from sklearn.model_selection import GridSearchCV
from sklearn.metrics import confusion_matrix, ConfusionMatrixDisplay
from sklearn.ensemble import RandomForestClassifier

class UserInstance:
  def __init__(self, ticker, exp_vars):
    self.ticker = ticker
    self.explanatory_vars = exp_vars
    self.dataframe = pd.DataFrame()

  def __repr__(self):
    return f"{self.ticker}:\n{self.dataframe.to_string()}"

# world industrial production
def WIP():
	# pull file with data
    file_url = 'https://drive.google.com/uc?export=download&id=1i5gl07FbEUoWQrIdSpB_ZWBcNlZuxm0R'
    file_response = requests.get(file_url)
	
    with open('industrial_prod.xlsx', 'wb') as file:
        file.write(file_response.content)

    df = pd.read_excel('industrial_prod.xlsx')

	# clean and return data
    col = df.iloc[0:, 1]
    col=col.rename("WIP")
    col=col.to_frame()
    col=col.drop(col.index[:180])
    col= col.reset_index(drop=True)
    return col

# Consumer Confidence Index
def CCI():
	# pull data
    file_url = 'https://stats.oecd.org/sdmx-json/data/DP_LIVE/OECD.CCI.AMPLITUD.LTRENDIDX.M/OECD?contentType=csv&detail=code&separator=comma&csv-lang=en&startPeriod=1974'
    file_response = requests.get(file_url)

    with open('cci.csv', 'wb') as file:
        file.write(file_response.content)

	# format
    df = pd.read_csv('cci.csv', on_bad_lines='skip')
    data = df.iloc[:, 6]
    data=data.tolist()
    nas=['NA']*12
    col = nas+data
    col = pd.DataFrame(col, columns=["Consumer confidence index, OECD"])

	# covert to numeric
    col['Consumer confidence index, OECD'] = pd.to_numeric(col['Consumer confidence index, OECD'], errors='coerce')
    return col

# geopolitical risk Indicator
def GPRI():
	# pull data
    file_url = 'https://www.matteoiacoviello.com/gpr_files/data_gpr_export.xls'
    file_response = requests.get(file_url)

    with open('gprh.xlsx', 'wb') as f:
        f.write(file_response.content)

    df = pd.read_excel('gprh.xlsx')
	
	# rows to add
    today = datetime.date.today()
    base = datetime.date(2023, 3, 1)
    rows_to_add = ((today.year - base.year) * 12 + (today.month - base.month))*-1
    new = df.iloc[rows_to_add:, 4]
    new=new.to_list()

    # combine fixed column values with the updates:
    # Specify the file path
    file_path = "GPRI_historical.txt"

    # Read the historical data from the file
    with open(file_path, 'r') as file:
      # Read the entire content of the file
      fdata = file.read()

    # Remove leading and trailing whitespace, and split the data into a list
    col = [float(value) for value in fdata.strip().split(',')]

	# append new data to historical data
    for i in new:
        col.append(i)

    col=pd.DataFrame({"Caldara-Iacoviello Geopolitical Risk Indicator": col})

    return col

# Consumer Sentiment Index
def CSI():
	# pull data
    file_url = 'http://www.sca.isr.umich.edu/files/tbmiccice.csv'
    file_response = requests.get(file_url)

    file_path = 'consumer_sentiment_index.xlsx'
    with open(file_path, 'wb') as file:
        file.write(file_response.content)

	# format data 
    df = pd.read_csv(file_path)
    data = df.iloc[92:, 3]
    data=data.tolist()
    nas=['NA']*60
    col=nas+data
    col=pd.DataFrame(col, columns=["index of consumer expectations"])

	# convert to numeric
    col['index of consumer expectations'] = pd.to_numeric(col['index of consumer expectations'], errors='coerce')
    return col

# Nominal Copper Price
def NCP():
	# pull data
    file_url = "https://thedocs.worldbank.org/en/doc/5d903e848db1d1b83e0ec8f744e55570-0350012021/related/CMO-Historical-Data-Monthly.xlsx"
    r = requests.get(file_url)

	# data by month
    datad = pd.read_excel(BytesIO(r.content), "Monthly Prices", skiprows=4, index_col=0)
    datad = datad[['Copper']]
    df = datad.tail(datad.shape[0] - 2)
    df.reset_index(inplace=True)
	
	# pull based on desired date
    df_copy = df.copy()
    df_copy['index'] = pd.to_datetime(df_copy['index'], format='%YM%m')
    df_copy['year'] = df_copy['index'].dt.year
    filtered_df = df_copy[df_copy['year'] > 1972]
    col = filtered_df['Copper'].to_list()
    col = pd.DataFrame(col, columns=['Nominal Copper Price'])
    return col

# Residential Energy Demand Index
def REDI():
	'''
	Data from this website can only be pull for a given month for all years. 
	Need to pull 12 files for each month of the year since 1973 and stack the dataframe
	so that each datapoint is ordered by date
	'''
    months = ['jan','feb','mar','apr','may','jun','jul','aug','sep','oct','nov','dec']
    dataframes = []
    dates = range(1973,datetime.date.today().year+1)
    df_date = pd.DataFrame(dates, columns=["Year"])
    dataframes.append(df_date)

	# pull each months data
    for mon in months:
        url = f"https://www.ncei.noaa.gov/access/monitoring/redti/USA/{mon}/1-month/data.csv"
        r = requests.get(url)
        df = pd.read_csv(BytesIO(r.content), usecols=["Date", "REDTI"], skiprows=1)
        df = df[df["Date"] > 1972]

        df_month = pd.DataFrame(df['REDTI'].to_list(), columns=[mon])

        dataframes.append(df_month)

	# combine data into one dataframe
    combined_df = pd.concat(dataframes, axis=1)
    combined_df = combined_df.loc[:, combined_df.columns != 'Year'].stack()

    col = []
    for year in combined_df:
        col.append(year)

    col = pd.DataFrame(col, columns=['Residential Energy Demand Temperature Index'])
    return col

# consumer price index
def CPI():
	# pull data
    try:
        r = requests.get("https://fred.stlouisfed.org/graph/fredgraph.csv?bgcolor=%23e1e9f0&chart_type=line&drp=0&fo=open%20sans&graph_bgcolor=%23ffffff&height=450&mode=fred&recession_bars=on&txtcolor=%23444444&ts=12&tts=12&width=718&nt=0&thu=0&trc=0&show_legend=yes&show_axis_titles=yes&show_tooltip=yes&id=CPIAUCSL&scale=left&cosd=1947-01-01&coed=2023-06-01&line_color=%234572a7&link_values=false&line_style=solid&mark_type=none&mw=3&lw=2&ost=-99999&oet=99999&mma=0&fml=a&fq=Monthly&fam=avg&fgst=lin&fgsnd=2020-02-01&line_index=1&transformation=lin&vintage_date=2023-07-22&revision_date=2023-07-22&nd=1947-01-01")
        r.raise_for_status()
    except requests.exceptions.HTTPError as errh:
        print("US CPI error: An Http Error occurred:" + repr(errh))
        exit(1)
    except requests.exceptions.ConnectionError as errc:
        print("US CPI error: An Error Connecting to the website occurred:" + repr(errc))
        exit(1)
    except requests.exceptions.Timeout as errt:
        print("US CPI error: A Timeout Error occurred:" + repr(errt))
        exit(1)
    except requests.exceptions.RequestException as err:
        print("US CPI error: An Unknown Connection Error occurred" + repr(err))
        exit(1)
	
	# clean and format
    df = pd.read_csv(BytesIO(r.content))
    df = df[df['DATE'] > '1972-12-31'].reset_index()
    col = pd.DataFrame(df, columns=['CPIAUCSL'])
    return col

# Nominal Brent Price
def NBP():
    # Specify the file path
    file_path = "NBP_historical.txt"

    # Read the data from the file
    with open(file_path, 'r') as file:
        # Read the entire content of the file
        fdata = file.read()

    # Remove leading and trailing whitespace, and split the data into a list
    known_data = [float(value) for value in fdata.strip().split(',')]

    try:
        r = requests.get("https://www.eia.gov/dnav/pet/hist_xls/RBRTEm.xls")
        r.raise_for_status()
    except requests.exceptions.HTTPError as errh:
        print("Nominal Brent Price Monthly error: An Http Error occurred:" + repr(errh))
        exit(1)
    except requests.exceptions.ConnectionError as errc:
        print("Nominal Brent Price Monthly error: An Error Connecting to the website occurred:" + repr(errc))
        exit(1)
    except requests.exceptions.Timeout as errt:
        print("Nominal Brent Price Monthly error: A Timeout Error occurred:" + repr(errt))
        exit(1)
    except requests.exceptions.RequestException as err:
        print("Nominal Brent Price Monthly error: An Unknown Connection Error occurred" + repr(err))
        exit(1)
    datam = pd.read_excel(BytesIO(r.content), "Data 1", skiprows=2)
    data_listm = datam['Europe Brent Spot Price FOB (Dollars per Barrel)'].to_list()
    known_data.extend(data_listm)

    # get last monthly value as a daily average for current month's observations
    try:
        r = requests.get("https://www.eia.gov/dnav/pet/hist_xls/RBRTEd.xls")
        r.raise_for_status()
    except requests.exceptions.HTTPError as errh:
        print("Nominal Brent Price Daily error: An Http Error occurred:" + repr(errh))
        exit(1)
    except requests.exceptions.ConnectionError as errc:
        print("Nominal Brent Price Daily error: An Error Connecting to the website occurred:" + repr(errc))
        exit(1)
    except requests.exceptions.Timeout as errt:
        print("Nominal Brent Price Daily error: A Timeout Error occurred:" + repr(errt))
        exit(1)
    except requests.exceptions.RequestException as err:
        print("Nominal Brent Price Daily error: An Unknown Connection Error occurred" + repr(err))
        exit(1)
	
	# clean and format by date
    datad = pd.read_excel(BytesIO(r.content), "Data 1", skiprows=2)
    datad["Date"] = pd.to_datetime(datad['Date'])
    datad['mnth_yr'] = datad['Date'].apply(lambda x: x.strftime('%B-%Y'))
    last_month = datad["mnth_yr"].iat[-1]
    last_month_value = pd.Series.mean(datad[datad["mnth_yr"] == last_month]['Europe Brent Spot Price FOB (Dollars per Barrel)'])
    known_data.append(last_month_value)

    col = pd.DataFrame(known_data, columns=['Nominal Brent Price'])

    return col

# OECD Petroleum Inventories
def OECD_PI():

    APIkey = '0fmKUqcOw1IouLEogcfq2CuczYUM4eFe9pZuEQUF'

    file_path = "OECD_PI_historical.txt"

    # Read the data from the file
    with open(file_path, 'r') as file:
        # Read the entire content of the file
        fdata = file.read()

    # Remove leading and trailing whitespace, and split the data into a list
    known_data = [float(value) for value in fdata.strip().split(',')]

    try:
        r = requests.get(f"https://api.eia.gov/v2/international/data/?frequency=monthly&data[0]=value&facets[activityId][]=5&facets[productId][]=5&facets[countryRegionId][]=OECD&facets[unit][]=MBBL&sort[0][column]=period&sort[0][direction]=desc&offset=0&length=5000&api_key={APIkey}")
        r.raise_for_status()
    except requests.exceptions.HTTPError as errh:
        print("OECD petroleum inventories error: An Http Error occurred:" + repr(errh))
        exit(1)
    except requests.exceptions.ConnectionError as errc:
        print("OECD petroleum inventories error: An Error Connecting to the API occurred:" + repr(errc))
        exit(1)
    except requests.exceptions.Timeout as errt:
        print("OECD petroleum inventories error: A Timeout Error occurred:" + repr(errt))
        exit(1)
    except requests.exceptions.RequestException as err:
        print("OECD petroleum inventories error: An Unknown Connection Error occurred" + repr(err))
        exit(1)

    data = r.json()
    df = pd.DataFrame(data['response']['data'])
    data_list = df['value'].to_list()
    known_data.extend(data_list[::-1])

    col = pd.DataFrame(known_data, columns=['OECD Petroleum Inventories'])
    return col

# Global Oil Production
def GOP():
    APIkey = '0fmKUqcOw1IouLEogcfq2CuczYUM4eFe9pZuEQUF'
    try:
        r = requests.get(f"https://api.eia.gov/v2/international/data/?frequency=monthly&data[0]=value&facets[activityId][]=1&facets[productId][]=57&facets[countryRegionId][]=WORL&facets[unit][]=TBPD&sort[0][column]=period&sort[0][direction]=desc&offset=0&length=5000&api_key={APIkey}")
        r.raise_for_status()
    except requests.exceptions.HTTPError as errh:
        print("Global Oil Production error: An Http Error occurred:" + repr(errh))
        exit(1)
    except requests.exceptions.ConnectionError as errc:
        print("Global Oil Production error: An Error Connecting to the API occurred:" + repr(errc))
        exit(1)
    except requests.exceptions.Timeout as errt:
        print("Global Oil Production error: A Timeout Error occurred:" + repr(errt))
        exit(1)
    except requests.exceptions.RequestException as err:
        print("Global Oil Production error: An Unknown Connection Error occurred" + repr(err))
        exit(1)
	
    data = r.json()
    df = pd.DataFrame(data['response']['data'])
    df = df[["period", "value"]]

	# data comes in opposite order so need to reverse
    data_list = df['value'].to_list()[::-1]
    data_list = list(map(lambda x: x / 1000, data_list))

    col = pd.DataFrame(data_list, columns=['Global Oil Production'])
    return col

# Nominal Oil Price
def NOP():
	# get data from 1973 till 1 month ago
    try:
        r = requests.get("https://fred.stlouisfed.org/graph/fredgraph.csv?bgcolor=%23e1e9f0&chart_type=line&drp=0&fo=open%20sans&graph_bgcolor=%23ffffff&height=450&mode=fred&recession_bars=on&txtcolor=%23444444&ts=12&tts=12&width=718&nt=0&thu=0&trc=0&show_legend=yes&show_axis_titles=yes&show_tooltip=yes&id=WTISPLC&scale=left&cosd=1946-01-01&coed=2023-07-01&line_color=%234572a7&link_values=false&line_style=solid&mark_type=none&mw=3&lw=2&ost=-99999&oet=99999&mma=0&fml=a&fq=Monthly&fam=avg&fgst=lin&fgsnd=2020-02-01&line_index=1&transformation=lin&vintage_date=2023-08-03&revision_date=2023-08-03&nd=1946-01-01")
        r.raise_for_status()
    except requests.exceptions.HTTPError as errh:
        print("WTI oil price error: An Http Error occurred:" + repr(errh))
        exit(1)
    except requests.exceptions.ConnectionError as errc:
        print("WTI oil price error: An Error Connecting to the website occurred:" + repr(errc))
        exit(1)
    except requests.exceptions.Timeout as errt:
        print("WTI oil price error: A Timeout Error occurred:" + repr(errt))
        exit(1)
    except requests.exceptions.RequestException as err:
        print("WTI oil price error: An Unknown Connection Error occurred" + repr(err))
        exit(1)
    df = pd.read_csv(BytesIO(r.content))
    df = df[df['DATE'] > '1972-12-31'].reset_index()
    data_list = df["WTISPLC"].to_list()

    # get last monthly value as a daily average for current month's observations
    try:
        r = requests.get("https://www.eia.gov/dnav/pet/hist_xls/RWTCd.xls")
        r.raise_for_status()
    except requests.exceptions.HTTPError as errh:
        print("WTI oil price daily error: An Http Error occurred:" + repr(errh))
        exit(1)
    except requests.exceptions.ConnectionError as errc:
        print("WTI oil price daily error: An Error Connecting to the website occurred:" + repr(errc))
        exit(1)
    except requests.exceptions.Timeout as errt:
        print("WTI oil price daily error: A Timeout Error occurred:" + repr(errt))
        exit(1)
    except requests.exceptions.RequestException as err:
        print("WTI oil price daily error: An Unknown Connection Error occurred" + repr(err))
        exit(1)

	# clean and format data
    datad = pd.read_excel(BytesIO(r.content), "Data 1", skiprows=2)
    datad["Date"] = pd.to_datetime(datad['Date'])
    datad['mnth_yr'] = datad['Date'].apply(lambda x: x.strftime('%B-%Y'))
    last_month = datad["mnth_yr"].iat[-1]
    last_month_value = pd.Series.mean(datad[datad["mnth_yr"] == last_month]['Cushing, OK WTI Spot Price FOB (Dollars per Barrel)'])
    data_list.append(round(last_month_value,2))

    col = pd.DataFrame(data_list, columns=['Nominal Oil Price'])
    return col

def pull_stock_data(ticker):
  # pull monthly stock data through twelve data api
  api_key = '8b3c2e3678304f1eacdbb8de0cfb6962'
  try:
      r = requests.get(f"https://api.twelvedata.com/time_series?apikey={api_key}&interval=1month&type=stock&format=JSON&start_date=1901-01-01 00:00:00&symbol={ticker}&previous_close=true")
      r.raise_for_status()
  except requests.exceptions.HTTPError as errh:
      print("Stock Pull error: An Http Error occurred:" + repr(errh))
      exit(1)
  except requests.exceptions.ConnectionError as errc:
      print("Stock Pull error: An Error Connecting to the website occurred:" + repr(errc))
      exit(1)
  except requests.exceptions.Timeout as errt:
      print("Stock Pull error: A Timeout Error occurred:" + repr(errt))
      exit(1)
  except requests.exceptions.RequestException as err:
      print("Stock Pull error: An Unknown Connection Error occurred" + repr(err))
      exit(1)

  # create dataframe
  data = r.json()
  df_stock = pd.DataFrame(data['values'])

  # create and isolate return column
  df_stock['return'] = (df_stock['close'].astype(float) - df_stock['previous_close'].astype(float)) / df_stock['previous_close'].astype(float)
  df_stock = df_stock[['datetime','return']]

  # reverse data order to match explanatory dataset
  df_stock = df_stock.iloc[::-1]

  return df_stock


def get_stock_input(api_key):
  # prompt the user for stock and check to make sure api supports it
  while True:
    ticker = input("Please input stock ticker for desired analysis: ")

    r = requests.get(f"https://api.twelvedata.com/time_series?apikey={api_key}&interval=1month&type=stock&format=JSON&symbol={ticker.upper()}&outputsize=1&dp=1")
    data = r.json()
	
	# error checking
    if "status" in data.keys() and data['status'] == "error":
      print(f"Error: Ticker <{ticker.upper()}> not available:\nCheck spelling or twelvedata api documentation and try again.")

      while True:
        circ_break = input("Would you like to try again (y/n): ")

        if circ_break.lower() == 'n':
          sys.exit(0)
        elif circ_break.lower() == 'y':
          break
        else:
          print("Please type 'y' (yes) or 'n' (no)")

      continue

    break

  return ticker

def get_explanatory_input():
  # use dictionary to track users desired variables. Initially set to False
  explanatory_vars = {
      'WIP' : {
          'name' : 'World Industrial Production',
          'include' : False,
          'function' : WIP
          },
      'CCI' : {
          'name' : 'Consumer Confidence Index',
          'include' : False,
          'function' : CCI
          },
      'GPRI' : {
          'name' : 'Geopolitical Risk Indicator',
          'include' : False,
          'function' : GPRI
          },
      'CSI' : {
          'name' : 'Consumer Sentiment Index',
          'include' : False,
          'function' : CSI
          },
      'NCP' : {
          'name' : 'Nominal Copper Price',
          'include' : False,
          'function' : NCP
          },
      'REDI' : {
          'name' : 'Residential Energy Demand Index',
          'include' : False,
          'function' : REDI
          },
      'CPI' : {
          'name' : 'Consumer Price Index',
          'include' : False,
          'function' : CPI
          },
      'NBP' : {
          'name' : 'Nominal Brent Price',
          'include' : False,
          'function' : NBP
          },
      'OECD_PI' : {
          'name' : 'Petroleum Inventories',
          'include' : False,
          'function' : OECD_PI
          },
      'GOP' : {
          'name' : 'Global Oil Production',
          'include' : False,
          'function' : GOP
          },
      'NOP' : {
          'name' : 'Nominal Oil Price',
          'include' : False,
          'function' : NOP
          }
  }
  
  # prompt user
  while True:
    flag = input(f"Would you like to include all available variables? (y/n): ").lower()

  	# error checking
    if flag == 'y':
      for var in explanatory_vars:
        explanatory_vars[var]['include'] = True

      return explanatory_vars
    elif flag == 'n':
      break
    else:
      print("Please respond with 'y' for Yes or 'n' for No")

  # if user did not select all variables, prompt for each one
  for var in explanatory_vars.keys():
    while True:
      flag = input(f"Would you like to include {explanatory_vars[var]['name']}? (y/n): ").lower()

      if flag == 'y':
        explanatory_vars[var]['include'] = True
        break
      elif flag == 'n':
        break
      else:
        print("Please respond with 'y' for Yes or 'n' for No")

  return explanatory_vars

def create_dataframe(ticker, exp_vars):
  # get dataframes for each requested variable

  exp_df_array = []

  for var in exp_vars.keys():
    if exp_vars[var]['include']:
      exp_df_array.append(exp_vars[var]['function']())

  # combine each dataset
  df_exp = pd.concat(exp_df_array, axis=1)

  # dates column
  start_date = datetime.datetime(1973, 1, 1)
  num_months = len(df_exp.index)
  date_range = pd.date_range(start=start_date, periods=num_months, freq='MS')

  df_date = pd.DataFrame({"datetime" : date_range})

  # add date column to dataset
  df_main = pd.concat([df_date, df_exp], axis=1)

  # get stock return data
  df_stock = pull_stock_data(ticker)
  df_stock['datetime']= pd.to_datetime(df_stock['datetime'])

  # combine dataframes
  df_merged = pd.merge(df_main, df_stock, on='datetime')

  # add direction column based on returns
  df_merged['direction'] = np.where(df_merged['return'] > 0, 'up', 'down')

  return df_merged

def exec_xgb(data):
  print("--------------------XGBoost--------------------")

  X = data.drop(['return', 'datetime', 'direction'], axis=1)  # Explanatory

  # up = 1, down = 0
  y = np.where(data['direction'] == 'up', 1, 0)

  # Split the dataset into training set and test set
  X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

  # Initialize the XGBoost classifier
  xgb_clf = xgb.XGBClassifier(objective='binary:logistic', random_state=1)

  # Fit the classifier to the training set
  xgb_clf.fit(X_train, y_train)

  # Predict the responses for the test set
  y_pred = xgb_clf.predict(X_test)

  # Evaluate the initial model
  accuracy = accuracy_score(y_test, y_pred)
  print(f"Initial Model Accuracy: {accuracy * 100:.2f}%")

  # Generate confusion matrix
  conf_matrix = confusion_matrix(y_test, y_pred)

  # Create heatmap of confusion matrix
  cm_display = ConfusionMatrixDisplay(conf_matrix).plot()

  # Plot feature importance
  xgb.plot_importance(xgb_clf, max_num_features = 10)
  plt.show()

  # xbg tuning
  X_train, X_val, y_train, y_val = train_test_split(X_train, y_train, test_size=0.2, random_state=42)

  eval_set = [(X_train, y_train), (X_val, y_val)]

  # Define a hyperparameter grid to search
  param_grid = {
    'max_depth': [3, 5, 7, 10], # Set values for max depth
    'min_child_weight': [1, 3, 10], # Set values for min child weight
    'learning_rate': [0.1], # Set learning rate
    'n_estimators': [100], # Set number of iterations
    'subsample': [1.0], # Set proportion of rows to use in each tree
    'colsample_bytree': [1.0], # Set proportion of columns to use in each tree
    'early_stopping_rounds':[10] # Set early stopping rounds
  }

  # Setup the grid search
  grid_search = GridSearchCV(
    estimator=xgb_clf, # Set estimator
    param_grid=param_grid, # Set parameter grid
    scoring='accuracy', # Set how tuning is done
    n_jobs=1, # Set number of jobs
    cv=5, # Set number of folds for cross validation
    verbose=0
  )

  grid_search.fit(X_train, y_train, eval_set=eval_set)

  # Best parameters and best score
  best_parameters = grid_search.best_params_

  xgb_clf = xgb.XGBClassifier(objective='binary:logistic', random_state=1,
                              early_stopping_rounds=best_parameters['early_stopping_rounds'],
                              colsample_bytree=best_parameters['colsample_bytree'],
                              learning_rate=best_parameters['learning_rate'],
                              max_depth=best_parameters['max_depth'],
                              min_child_weight=best_parameters['min_child_weight'],
                              n_estimators=best_parameters['n_estimators'],
                              subsample=best_parameters['subsample'])

  # Fit the classifier to the training set
  xgb_clf.fit(X_train, y_train, eval_set=eval_set)

  # Predict the responses for the test set
  y_pred = xgb_clf.predict(X_test)

  # Evaluate the initial model
  accuracy = accuracy_score(y_test, y_pred)
  print(f"Tuned Model Accuracy: {accuracy * 100:.2f}%")

  # Generate confusion matrix
  conf_matrix = confusion_matrix(y_test, y_pred)

  # Create heatmap of confusion matrix
  cm_display = ConfusionMatrixDisplay(conf_matrix).plot()

  # Plot feature importance
  xgb.plot_importance(xgb_clf, max_num_features = 10)
  plt.show()


# Create metric calculation function
def calculate_metrics(y_true, y_pred):
    tn, fp, fn, tp = confusion_matrix(y_true, y_pred).ravel()

    # calculate metrics
    accuracy = (tp + tn) / (tp + tn + fp + fn)
    balanced_accuracy = (0.5 * (tp / (tp + fn) + tn / (tn + fp)))
    sensitivity = tp / (tp + fn)
    specificity = tn / (tn + fp)

    return accuracy, balanced_accuracy, sensitivity, specificity

def exec_random_forest(data):
  print("--------------------Random Forest--------------------")

  # Training Data
  X_train = data.drop(['return', 'datetime', 'direction'], axis=1)  # Explanatory
  # up = 1, down = 0
  y_train = np.where(data['direction'] == 'up', 1, 0)

  # Testing Data
  X_test = data.drop(['return', 'datetime', 'direction'], axis=1)  # Explanatory
  # up = 1, down = 0
  y_test = np.where(data['direction'] == 'up', 1, 0)

  # Initialise model
  model = RandomForestClassifier(random_state = 42, n_estimators = 25, max_features = 'sqrt', max_depth = 3, max_leaf_nodes = 9)

  # Fit model to training data
  model.fit(X_train, y_train)

  # predict for the test set
  y_pred = model.predict(X_test)

  # Generate confusion matrix
  conf_matrix = confusion_matrix(y_test, y_pred)

  # Create heatmap of confusion matrix
  cm_display = ConfusionMatrixDisplay(conf_matrix).plot()

  # Calculate performance metrics:
  accuracy_rf1, balanced_accuracy_rf1, sensitivity_rf1, specificity_rf1 = calculate_metrics(y_test, y_pred)
  # Print out metrics
  print(f'Accuracy: {accuracy_rf1}')
  print(f'Balanced Accuracy: {balanced_accuracy_rf1}')
  print(f'Sensitivity: {sensitivity_rf1}')
  print(f'Specificity: {specificity_rf1}')

def main():
  api_key = '8b3c2e3678304f1eacdbb8de0cfb6962'

  # prompt user for ticker and check response
  ticker = get_stock_input(api_key)

  # get desired explanatory variables
  exp_vars = get_explanatory_input()
  UI = UserInstance(ticker, exp_vars)

  # create dataframe with response variable
  UI.dataframe = create_dataframe(ticker, exp_vars)
  UI.dataframe.dropna(inplace=True)

  # XGBoost
  exec_xgb(UI.dataframe)

  # Random Forest
  exec_random_forest(UI.dataframe)

if __name__ == "__main__":
  main()
