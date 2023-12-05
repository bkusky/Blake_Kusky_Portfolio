#!/usr/bin/env python3

import csv

def main():
	
	files = ["burger-king-menu.csv", "DunkinDonutsNutrition.csv", "LittleCaesarsPizzaMenuNutrition.csv", "McDonaldsMenuNutritionV2.csv", "SubwayMenuNutrition.csv", "wendys-menu.csv"]	

	data = []
		
	for file in files:

		with open(file, 'r') as f:
			r = csv.reader(f)

			if file == "burger-king-menu.csv":
				for index, row in enumerate(r):
					if index == 0:
						continue	

					rest = "Burger King"
					
					if row[12] == "":
						continue

					data.append([rest, row[0], row[2], row[4], row[5], row[6], row[7], row[8], row[9], row[10], row[11], row[12], row[13]])
	
			elif file == "DunkinDonutsNutrition.csv":
				for index, row in enumerate(r):
					if index == 0:
						continue	

					rest = "Dunkin Donuts"

					if not row[3].isnumeric():
						continue
					
					if row[12] == "":
						continue

					data.append([rest, row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9], row[10], row[11], row[12]])

			elif file == "LittleCaesarsPizzaMenuNutrition.csv":
				for index, row in enumerate(r):
					if index == 0:
						continue	

					rest = "Little Caesars"
					
					if row[12] == "":
						continue

					data.append([rest, row[0], row[1], row[3], row[4], row[5], row[6], row[7], row[8], row[9], row[10], row[11], row[12]])

			elif file == "McDonaldsMenuNutritionV2.csv":
				for index, row in enumerate(r):
					if index == 0:
						continue	

					rest = "McDonalds"

					if row[12] == "":
						continue

					data.append([rest, row[0], row[1], row[3], row[4], row[5], row[6], row[7], row[8], row[9], row[10], row[11], row[12]])

			elif file == "SubwayMenuNutrition.csv":
				for index, row in enumerate(r):
					if index == 0:
						continue	

					rest = "Subway"

					if row[12] == "":
						continue

					data.append([rest, row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9], row[10], row[11], row[12]])

			elif file == "wendys-menu.csv":
				for index, row in enumerate(r):
					if index == 0:
						continue	

					rest = "Wendys"

					if row[12] == "":
						continue

					data.append([rest, row[0], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9], row[10], row[11], row[12]])

					
	fields = ["Restaurant", "Item", "Calories", "Fat (g)", "Saturated Fat (g)", "Trans Fat (g)", "Cholesterol (mg)", "Sodium (mg)", "Carbs (g)", "Fiber (g)", "Sugars (g)", "Protein (g)", "Weight Watcher Pnts"]

	for line in data:
		if line[6] == "<5":
			line[6] = "2.5"

	with open('FastFoodData.csv', 'w') as f:  
		write = csv.writer(f)
    	
		write.writerow(fields)
		write.writerows(data)

if __name__ == "__main__":
	main()
