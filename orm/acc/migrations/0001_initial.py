# Generated by Django 3.0.2 on 2020-03-16 04:01

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Color',
            fields=[
                ('color_id', models.AutoField(primary_key=True, serialize=False)),
                ('name', models.CharField(max_length=256)),
            ],
        ),
        migrations.CreateModel(
            name='State',
            fields=[
                ('state_id', models.AutoField(primary_key=True, serialize=False)),
                ('name', models.CharField(max_length=256)),
            ],
        ),
        migrations.CreateModel(
            name='Team',
            fields=[
                ('team_id', models.AutoField(primary_key=True, serialize=False)),
                ('name', models.CharField(max_length=256)),
                ('wins', models.IntegerField()),
                ('losses', models.IntegerField()),
                ('color', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='acc.Color')),
                ('state', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='acc.State')),
            ],
        ),
        migrations.CreateModel(
            name='Player',
            fields=[
                ('player_id', models.AutoField(primary_key=True, serialize=False)),
                ('uniform_num', models.IntegerField()),
                ('first_name', models.CharField(max_length=256)),
                ('last_name', models.CharField(max_length=256)),
                ('mpg', models.IntegerField()),
                ('ppg', models.IntegerField()),
                ('rpg', models.IntegerField()),
                ('apg', models.IntegerField()),
                ('spg', models.FloatField()),
                ('bpg', models.FloatField()),
                ('team', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='acc.Team')),
            ],
        ),
    ]
