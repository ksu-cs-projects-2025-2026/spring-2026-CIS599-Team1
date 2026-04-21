import os
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# 1. Setup relative paths
script_dir = os.path.dirname(os.path.abspath(__file__))
file_path = os.path.join(script_dir, "model_results.csv")
output_file = os.path.join(script_dir, "heatmap.png")

# 2. Load and Filter
df = pd.read_csv(file_path)
df_mean = df[df['Bound_Type'] == 'Mean']

# 3. Pivot to Grid (Models on Y-axis, Entropy on X-axis)
pivot_df = df_mean.pivot(index='Model', columns='Expected_Min_Entropy', values='Value')

# 4. Calculate Absolute Deviation from the Target Entropy
deviation_df = pivot_df.copy()
for col in deviation_df.columns:
    deviation_df[col] = (deviation_df[col] - float(col)).abs()

# 5. Create the Heatmap to match your reference image
plt.figure(figsize=(16, 10))
sns.heatmap(
    deviation_df, 
    annot=True, 
    fmt=".4f", 
    cmap="Reds",      # Red color scale as seen in your image
    linewidths=.5,
    cbar_kws={'label': 'Absolute Deviation'}
)

# 6. Add titles and labels matching your image
plt.title('Model Deviation from Expected Entropy', fontsize=20, pad=20)
plt.ylabel(None) # Your image doesn't have a specific Y label
plt.xlabel('Target Entropy Level', fontsize=12)

# 7. Save and Show
plt.savefig(output_file, dpi=300, bbox_inches='tight')
print(f"Success! Heatmap saved to: {output_file}")
plt.show()