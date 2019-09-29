using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace task3
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        List<Thread> threads;
        public MainWindow()
        {
            InitializeComponent();
            threads = new List<Thread>();
        }

        TextBox NewTextBox()
        {
            var textBox = new TextBox();
            textBox.IsReadOnly = true;
            textBox.Margin = new Thickness(0, 0, 0, 3);
            textBox.Padding = new Thickness(0, 0, 0, 4);
            textBox.VerticalContentAlignment = VerticalAlignment.Center;
            textBox.FontSize = 14;
            textBox.BorderThickness = new Thickness(0);
            return textBox;
        }

        void StopAndClear()
        {
            foreach (var thread in threads)
            {
                thread.Abort();
            }
            stackPanel.Children.Clear();
        }

        private void button_Click(object sender, RoutedEventArgs e)
        {
            if (stackPanel.Children.Count >= 7)
            {
                StopAndClear();
            }

            try
            {
                var textBox = NewTextBox();

                var eps = double.Parse(epsilonInput.Text);
                if (eps <= 0)
                {
                    throw new FormatException("Epsilon must be greater then zero.");
                }

                textBox.Text = $"Eps: {eps:F10}\t    Calculating...";
                stackPanel.Children.Add(textBox);

                var thread = new Thread(() =>
                {
                    var calc = new Calc(eps);
                    calc.CalcPi();
                    Dispatcher.Invoke(() =>
                    {
                        textBox.Text = $"Eps: {eps:F10}\t    Result: {calc.Result:F14}";
                        textBox.Background = Brushes.LightGreen;
                    });
                });
                threads.Add(thread);
                thread.Start();
            }
            catch (Exception ex)
            {
                var errorWindow = new ErrorWindow(ex.Message);
                errorWindow.ShowDialog();
            }
        }

        private void stopButton_Click(object sender, RoutedEventArgs e)
        {
            StopAndClear();
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);
            StopAndClear();
        }
    }
}
