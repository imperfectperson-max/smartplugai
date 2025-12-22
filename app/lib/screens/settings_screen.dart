import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../services/auth_service.dart';

class SettingsScreen extends StatelessWidget {
  const SettingsScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final authService = Provider.of<AuthService>(context);
    final user = authService.currentUser;

    return Scaffold(
      appBar: AppBar(
        title: const Text('Settings'),
      ),
      body: ListView(
        children: [
          // User profile section
          Container(
            padding: const EdgeInsets.all(24),
            child: Column(
              children: [
                CircleAvatar(
                  radius: 40,
                  backgroundColor: const Color(0xFF2563EB),
                  child: Text(
                    user?.fullName.substring(0, 1).toUpperCase() ?? 'U',
                    style: const TextStyle(
                      fontSize: 32,
                      color: Colors.white,
                      fontWeight: FontWeight.bold,
                    ),
                  ),
                ),
                const SizedBox(height: 16),
                Text(
                  user?.fullName ?? 'User',
                  style: const TextStyle(
                    fontSize: 20,
                    fontWeight: FontWeight.bold,
                  ),
                ),
                Text(
                  user?.email ?? '',
                  style: TextStyle(
                    fontSize: 14,
                    color: Colors.grey[600],
                  ),
                ),
              ],
            ),
          ),

          const Divider(),

          // Security section
          _buildSectionHeader('Security'),
          _buildSettingTile(
            icon: Icons.lock,
            title: 'Two-Factor Authentication',
            subtitle: user?.twoFactorEnabled ?? false
                ? 'Enabled'
                : 'Not enabled',
            trailing: Switch(
              value: user?.twoFactorEnabled ?? false,
              onChanged: (value) {
                // TODO: Enable/disable 2FA
              },
            ),
            onTap: () {},
          ),
          _buildSettingTile(
            icon: Icons.vpn_key,
            title: 'Change Password',
            subtitle: 'Update your password',
            onTap: () {
              // TODO: Navigate to change password screen
            },
          ),

          const Divider(),

          // App settings section
          _buildSectionHeader('App Settings'),
          _buildSettingTile(
            icon: Icons.notifications,
            title: 'Notifications',
            subtitle: 'Manage notification preferences',
            onTap: () {
              // TODO: Navigate to notifications settings
            },
          ),
          _buildSettingTile(
            icon: Icons.palette,
            title: 'Theme',
            subtitle: 'Light / Dark mode',
            onTap: () {
              // TODO: Theme selector
            },
          ),

          const Divider(),

          // About section
          _buildSectionHeader('About'),
          _buildSettingTile(
            icon: Icons.info,
            title: 'Version',
            subtitle: '0.1.0 (Pre-Hardware)',
            onTap: () {},
          ),
          _buildSettingTile(
            icon: Icons.security,
            title: 'Security',
            subtitle: 'Bank-grade encryption enabled',
            onTap: () {},
          ),
          _buildSettingTile(
            icon: Icons.description,
            title: 'Terms & Privacy',
            subtitle: 'View terms and privacy policy',
            onTap: () {
              // TODO: Show terms and privacy
            },
          ),

          const SizedBox(height: 32),

          // Mock mode indicator
          Container(
            margin: const EdgeInsets.all(16),
            padding: const EdgeInsets.all(16),
            decoration: BoxDecoration(
              color: Colors.blue.shade50,
              borderRadius: BorderRadius.circular(8),
            ),
            child: Column(
              children: [
                const Text(
                  'Pre-Hardware Mode',
                  style: TextStyle(
                    fontWeight: FontWeight.bold,
                    fontSize: 14,
                  ),
                ),
                const SizedBox(height: 8),
                Text(
                  'Using mock data for development.\nReal backend integration pending.',
                  style: TextStyle(fontSize: 12, color: Colors.grey[700]),
                  textAlign: TextAlign.center,
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildSectionHeader(String title) {
    return Padding(
      padding: const EdgeInsets.fromLTRB(16, 16, 16, 8),
      child: Text(
        title,
        style: const TextStyle(
          fontSize: 14,
          fontWeight: FontWeight.bold,
          color: Color(0xFF2563EB),
        ),
      ),
    );
  }

  Widget _buildSettingTile({
    required IconData icon,
    required String title,
    required String subtitle,
    Widget? trailing,
    VoidCallback? onTap,
  }) {
    return ListTile(
      leading: Icon(icon, color: const Color(0xFF2563EB)),
      title: Text(title),
      subtitle: Text(subtitle),
      trailing: trailing ?? const Icon(Icons.chevron_right),
      onTap: onTap,
    );
  }
}
