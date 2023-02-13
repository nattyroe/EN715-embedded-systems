import glob
import os

import numpy as np
import pandas as pd

from bokeh.plotting import ColumnDataSource, figure, show

# Find all log files -  end in .TXT extension
file_list = glob.glob('*.TXT')

# Create plot for each file
for file in file_list:
    file_name = os.path.splitext(os.path.basename(file))[0]

    x_axis_label = 'Time (s)'
    y_axis_label = 'Temp (F)'

    # Read in CSV data
    vals = pd.read_csv(file, header=None, names=[x_axis_label, y_axis_label])

    # Set up color array - 0F->#0000FF, 100F->#FF0000
    reds = np.array(['0x{:02x}'.format(int(255*temp/100))
                     for temp in vals[y_axis_label]])
    reds[vals[y_axis_label] >= 100] = "0xFF"

    blues = np.array(['0x{:02x}'.format(int(255 - (255*temp/100)))
                      for temp in vals[y_axis_label]])
    blues[vals[x_axis_label] <= 0] = "0xFF"

    colors = [
        f'#{reds[idx].split("x")[1]}00{blues[idx].split("x")[1]}' for idx in range(len(reds))]

    # Begin Bokeh plotting
    source = ColumnDataSource({'Time': vals[x_axis_label],
                               'Temp': vals[y_axis_label],
                               'colors': colors})

    TOOLTIPS = [(x_axis_label, '@Time'),
                (y_axis_label, '@Temp')]

    tools = 'hover,wheel_zoom,pan,box_zoom,reset,undo,save'

    fig = figure(title=f'Arduino Temperature Log - {file_name}',
                 toolbar_location="right",
                 aspect_ratio=2,
                 tools=tools,
                 tooltips=TOOLTIPS)

    fig.xaxis.axis_label = x_axis_label
    fig.yaxis.axis_label = y_axis_label

    fig.scatter('Time',
                'Temp',
                source=source,
                fill_color='colors',
                line_color='colors')
    show(fig)
