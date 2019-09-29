using System.Windows;

namespace task4
{
    /// <summary>
    /// Interaction logic for ErrorWindow.xaml
    /// </summary>
    public partial class ErrorWindow : Window
    {
        public ErrorWindow(string errorMessage)
        {
            InitializeComponent();
            errorMessageLabel.Content = errorMessage;
        }
    }
}
