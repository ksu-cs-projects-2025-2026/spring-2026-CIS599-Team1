import os
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# Define the correct names and order based
correct_names_order = [
    "All Models (MEM=6)",
    "No Bitwise/MatchChar",
    "PAQ6v2 w/ SSE",
    "BiasAware Char Model",
    "Bitwise Char Model",
    "Char Rev Model",
    "CharComplement Model",
    "MatchChar Model",
    "Original 90B Estimators",
    "LZ78Y bit",
    "Lag Bit",
    "Multi Markov Model Bit",
    "MultiMCW Bit"
]

# 1. Setup relative paths
script_dir = os.path.dirname(os.path.abspath(__file__))
file_path = os.path.join(script_dir, "model_results.csv")
output_file = os.path.join(script_dir, "heatmap.png")

# 2. Load and Filter
df = pd.read_csv(file_path)
df_mean = df[df['Bound_Type'] == 'Mean']
df_mean['Model'] = pd.Categorical(df_mean['Model'], categories=correct_names_order, ordered=True)

# 3. Pivot to Grid (Models on Y-axis, Entropy on X-axis)
pivot_df = df_mean.pivot(index='Model', columns='Expected_Min_Entropy', values='Value')

# 4. Calculate SIGNED Deviation (Actual - Target)
signed_deviation_df = pivot_df.copy()
for col in signed_deviation_df.columns:
    # Removed .abs() to keep the sign
    signed_deviation_df[col] = signed_deviation_df[col] - float(col)

# 5. Create the Heatmap to match your reference image
plt.figure(figsize=(16, 10))
sns.heatmap(
    signed_deviation_df, 
    annot=True,
    vmax="0.08",
    fmt=".4f", 
    cmap="RdBu_r",
    linewidths=.5,
    cbar_kws={'label': 'Estimated - Target'}
)

# 6. Add titles and labels matching your image
plt.title('Model Deviation from Expected Entropy', fontsize=20, pad=20)
plt.ylabel(None) # image doesn't have a specific Y label
plt.xlabel('Target Entropy Level', fontsize=12)

# 7. Save and Show
plt.savefig(output_file, dpi=300, bbox_inches='tight')
print(f"Success! Heatmap saved to: {output_file}")
plt.show()